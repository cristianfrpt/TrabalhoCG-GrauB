#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

#include "SceneManager.hpp"


int main() {
	SceneManager* s = new SceneManager();

    s->initialize();
	return s->run();
}