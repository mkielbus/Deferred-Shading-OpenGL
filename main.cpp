#include <iostream>
#include <fstream>
#include <vector>

#define GLEW_STATIC
#include "SingletonApp.h"

int main()
{
    SingletonApp &app = SingletonApp::getInstance();

    app.prepareWindow();

    app.prepareScene();

    app.execute();

    return 0;
}