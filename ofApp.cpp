#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	ofSetCircleResolution(72);

	ofSetBackgroundAuto(false);

	this->number_of_targets = 150;
	for (int i = 0; i < this->number_of_targets; i++) {

		this->target_list.push_back(glm::vec2());
		this->color_list.push_back(glm::vec3(1, 1, 1));
	}

	this->shader.load("shader/shader.vert", "shader/shader.frag");
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	if (ofGetFrameNum() % 180 == 0) {
		
		ofBackground(0);
	}

	ofSetColor(255);
	vector<pair<glm::vec2, float>> circles;
	circles.push_back(make_pair(glm::vec2(ofGetWidth() * 0.5, ofGetHeight() * 0.5), 230));
	for (int i = 0; i < 23; i++) {

		auto location = glm::vec2(ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.0035), 0, 1, 50, ofGetWidth() - 50), ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.0035), 0, 1, 50, ofGetHeight() - 50));
		auto radius = ofRandom(30, 60);

		auto circle = make_pair(location, radius);
		circles.push_back(circle);
	}

	vector<glm::vec2> tmp_target_list;
	vector<glm::vec3> tmp_color_list;
	for (auto circle : circles) {

		int p_1 = ofRandom(3);
		for (auto other : circles) {

			int p_2 = ofRandom(3);
			if (circle == other) { continue; }

			double theta, a;
			if (this->calculate_angle(circle.first, circle.second, other.first, other.second, theta, a)) {

				auto point_1 = circle.first + glm::vec2(circle.second * cos(theta + a), circle.second * sin(theta + a));
				auto point_2 = circle.first + glm::vec2(circle.second * cos(theta - a), circle.second * sin(theta - a));

				tmp_target_list.push_back(point_1);
				tmp_target_list.push_back(point_2);

				glm::vec3 tmp_color;
				switch (p_1)
				{
				case 0:
					tmp_color += glm::vec3(0.3, 0.2, 0.2);
					break;
				case 1:
					tmp_color += glm::vec3(0.2, 0.3, 0.2);
					break;
				case 2:
					tmp_color += glm::vec3(0.2, 0.2, 0.3);
					break;
				}

				switch (p_2)
				{
				case 0:
					tmp_color += glm::vec3(0.3, 0.2, 0.2);
					break;
				case 1:
					tmp_color += glm::vec3(0.2, 0.3, 0.2);
					break;
				case 2:
					tmp_color += glm::vec3(0.2, 0.2, 0.3);
					break;
				}

				tmp_color_list.push_back(tmp_color);
			}
		}
	}

	for (int i = 0; i < this->number_of_targets; i++) {

		if (i < tmp_target_list.size()) {

			this->target_list[i] = tmp_target_list[i];
			this->color_list[i] = tmp_color_list[i / 2];
		}
		else {

			this->target_list[i] = glm::vec2();
			this->color_list[i] = glm::vec3();
		}
	}

	ofFill();
	this->shader.begin();
	this->shader.setUniform1f("time", ofGetElapsedTimef());
	this->shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	this->shader.setUniform2fv("targets", &this->target_list[0].x, this->number_of_targets);
	this->shader.setUniform3fv("colors", &this->color_list[0].x, this->number_of_targets);

	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	this->shader.end();
}

//--------------------------------------------------------------
bool ofApp::calculate_angle(glm::vec2 location_1, float radius_1, glm::vec2 location_2, float radius_2, double& theta, double& a) {

	auto distance = glm::distance(location_1, location_2);
	if (distance > radius_1 + radius_2 || distance < radius_1 - radius_2) { return false; }

	theta = atan2(location_2.y - location_1.y, location_2.x - location_1.x);
	auto cos_a = (pow(distance, 2) + pow(radius_1, 2) - pow(radius_2, 2)) / (2 * distance * radius_1);
	a = acos(cos_a);
	return true;
}

//--------------------------------------------------------------
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(720, 720);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}