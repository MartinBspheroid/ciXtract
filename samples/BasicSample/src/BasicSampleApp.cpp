/*
 *  BasicSampleApp.cpp
 *
 *  Created by Andrea Cuius
 *  Nocte Studio Ltd.
 *
 *  www.nocte.co.uk
 *
 */


#include "cinder/app/AppNative.h"

#include "ciXtract.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define WIDGET_SIZE Vec2i( 120, 40 )

class BasicSampleApp : public AppBasic {

public:
	
	void prepareSettings( Settings *settings );
	void setup();
	void update();
    void draw();
	
	void drawPcmData();
    void drawFeature( ciXtractFeatureRef feature );
	void drawData( string label, std::shared_ptr<double> data, int N, float gain, Rectf rect);
    
    audio::Input                mInput;
    ciXtractRef                 mXtract;
	
    vector<ciXtractFeatureRef>  mFeatures;
};


void BasicSampleApp::prepareSettings(Settings *settings)
{
	settings->setTitle("ciXtract Sample");
	settings->setWindowSize( 900, 700 );
}


void BasicSampleApp::setup()
{
    const std::vector<audio::InputDeviceRef>& devices = audio::Input::getDevices();
	for( std::vector<audio::InputDeviceRef>::const_iterator iter = devices.begin(); iter != devices.end(); ++iter )
    {
        if ( (*iter)->getName() == "Soundflower (2ch)" )
        {
            mInput = audio::Input( *iter );
            mInput.start();
            break;
        }
	}
 
    if ( !mInput )
        exit(-1);
 
    mXtract     = ciXtract::create( mInput );
    mFeatures   = mXtract->getFeatures();
    
    for( auto k=0; k < XTRACT_FEATURES; k++ )
        mXtract->enableFeature( (xtract_features_)k );
}


void BasicSampleApp::update()
{
	mXtract->update();
}


void BasicSampleApp::draw()
{
	gl::clear( Color::gray( 0.85f ) ); 	
    gl::enableAlphaBlending();
    
	gl::color( Color::gray( 0.1f ) );
	drawPcmData();
    
    Rectf               rect;
    Vec2f               initPos( 15, 100 );
    Vec2f               pos = initPos;
    ciXtractFeatureRef  feature;
    
    for( auto k=0; k < mFeatures.size(); k++ )
    {
        feature = mFeatures[k];
        rect    = Rectf( pos, pos + WIDGET_SIZE );
        
        if ( feature->getType() == CI_XTRACT_SCALAR )
            drawData( feature->getName(), feature->getResult(), 1, 1.0f, rect );
     
        else if ( mFeatures[k]->getType() == CI_XTRACT_VECTOR )
            drawData( feature->getName(), feature->getResult(), feature->getResultN(), 100.0f, rect );
        
        pos.y += WIDGET_SIZE.y + 25;
        
        if ( pos.y >= getWindowHeight() - WIDGET_SIZE.y )
        {
            pos.x += WIDGET_SIZE.x + initPos.x;
            pos.y = initPos.y;
        }
    }
}


void BasicSampleApp::drawPcmData()
{
    audio::PcmBuffer32fRef pcmBuffer = mInput.getPcmBuffer();
 
	if( !pcmBuffer )
		return;
 
	uint32_t bufferLength           = pcmBuffer->getSampleCount();
	audio::Buffer32fRef leftBuffer  = pcmBuffer->getChannelData( audio::CHANNEL_FRONT_LEFT );
 
	int     displaySize = getWindowWidth();
	float   scale       = displaySize / (float)bufferLength;

	PolyLine<Vec2f>	leftBufferLine;

	for( int i = 0; i < bufferLength; i++ )
    {
		float x = i * scale;
        float y = 50 + leftBuffer->mData[i] * 60;
		leftBufferLine.push_back( Vec2f( x , y) );
	}

	gl::draw( leftBufferLine );
}


void BasicSampleApp::drawData( string label, std::shared_ptr<double> data, int N, float gain, Rectf rect )
{
    Color col = Color::white();
    
    glPushMatrix();
    
    gl::drawString( label, rect.getUpperLeft(), Color::black() );
    
    rect.y1 += 10;
    
    gl::color( col * 0.2f );
    gl::drawSolidRect( rect );
    
    gl::translate( rect.getUpperLeft() );
    
    glBegin( GL_QUADS );

    float step = rect.getWidth() / N;
    float h = rect.getHeight();

    for( int i = 0; i < N; i++ )
    {
        float barY  = data.get()[i] * gain;
        barY        = math<float>::clamp( barY, 0.0f, h );

        gl::color( col );
        glVertex2f( i * step,           h );
        glVertex2f( ( i + 1 ) * step,   h );
        glVertex2f( ( i + 1 ) * step,   h-barY );
        glVertex2f( i * step,           h-barY );
    }

    glEnd();

    gl::popMatrices();
}


CINDER_APP_BASIC( BasicSampleApp, RendererGl )

