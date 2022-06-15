#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/app/MouseEvent.h"
#include "imgui/imgui.h"

using namespace ci;
using namespace ci::app;

struct color {
	float r;
	float g;
	float b;
};

enum class Shapes {
	CIRCLE,
	SQUARE,
	RECTANGLE
};

class Shape {
public:
	vec2 location;
	color myColor;
	Shapes shape;
};

class Circle : public Shape {
public:
	float radius;

	Circle(vec2 loc, Shapes s) {
		location = loc;
		shape = s;

		myColor.r = randFloat(1);
		myColor.g = randFloat(1);
		myColor.b = randFloat(1);

		radius = randFloat(100);
	}
};

class Square : public Shape {
public:
	float size;
};

class Rectangle : public Shape {
public:
	float width;
	float height;
};



// We'll create a new Cinder Application by deriving from the App class.
class BasicApp : public App {
  public:
	// Cinder will call 'mouseDrag' when the user moves the mouse while holding one of its buttons.
	// See also: mouseMove, mouseDown, mouseUp and mouseWheel.
	//void mouseDrag( MouseEvent event ) override;

	void mouseDown(MouseEvent event) override;

	// Cinder will call 'keyDown' when the user presses a key on the keyboard.
	// See also: keyUp.
	void keyDown( KeyEvent event ) override;

	// Cinder will call 'draw' each time the contents of the window need to be redrawn.
	void draw() override;

	~BasicApp() {
		for (const Circle* circle : shapePositions) {
			delete circle;
		}
	}

  private:
	// This will maintain a list of points which we will draw line segments between
	std::vector<Circle*> shapePositions;
};

void prepareSettings( BasicApp::Settings* settings )
{
	settings->setMultiTouchEnabled( false );

}

void BasicApp::mouseDown( MouseEvent event )
{
	if (event.isRightDown()) {
		Circle* circleToDelete = NULL;
		for (Circle* circle : shapePositions) {
			if ((pow((circle->location.x - event.getX()), 2) + pow((circle->location.y - event.getY()), 2)) <= pow(circle->radius, 2)) 
			{
				circleToDelete = circle;
			}
		}
		shapePositions.erase(std::remove(shapePositions.begin(), shapePositions.end(), circleToDelete), shapePositions.end());
	}
	else {
		shapePositions.push_back(new Circle(event.getPos(), Shapes::CIRCLE));
	}
	
}

void BasicApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'f' ) {
		// Toggle full screen when the user presses the 'f' key.
		setFullScreen( ! isFullScreen() );
	}
	else if( event.getCode() == KeyEvent::KEY_SPACE ) {
		// Clear the list of points when the user presses the space bar.
		shapePositions.clear();
	}
	else if( event.getCode() == KeyEvent::KEY_ESCAPE ) {
		// Exit full screen, or quit the application, when the user presses the ESC key.
		if( isFullScreen() )
			setFullScreen( false );
		else
			quit();
	}
	else if (event.getChar() == 'w' && shapePositions.size() > 0) {
			Circle* currentCircle = shapePositions.back();
			currentCircle->location.y -= 5;
	}
	else if (event.getChar() == 's' && shapePositions.size() > 0) {
		Circle* currentCircle = shapePositions.back();
		currentCircle->location.y += 5;
	}
	else if (event.getChar() == 'a' && shapePositions.size() > 0) {
		Circle* currentCircle = shapePositions.back();
		currentCircle->location.x -= 5;
	}
	else if (event.getChar() == 'd' && shapePositions.size() > 0) {
		Circle* currentCircle = shapePositions.back();
		currentCircle->location.x += 5;
	}
	

}



void BasicApp::draw()
{
	// Clear the contents of the window. This call will clear
	// both the color and depth buffers. 
	gl::clear( Color::gray( 0.1f ) );

	// Set the current draw color to orange by setting values for
	// red, green and blue directly. Values range from 0 to 1.
	// See also: gl::ScopedColor
	

	// We're going to draw a line through all the points in the list
	// using a few convenience functions: 'begin' will tell OpenGL to
	// start constructing a line strip, 'vertex' will add a point to the
	// line strip and 'end' will execute the draw calls on the GPU.
	gl::begin( GL_LINE_STRIP );
	for( const Circle * circle : shapePositions) {
		gl::color(circle->myColor.r, circle->myColor.g, circle->myColor.b);
		gl::drawSolidCircle(circle->location, circle->radius);
	}

	gl::end();
}

// This line tells Cinder to actually create and run the application.
CINDER_APP( BasicApp, RendererGl, prepareSettings )
