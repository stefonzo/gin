#include "gin.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main() {
    gin application(WINDOW_WIDTH, WINDOW_HEIGHT, "Test App");
    application.loop();
    return 0;
}