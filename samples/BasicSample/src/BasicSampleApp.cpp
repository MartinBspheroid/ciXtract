#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

#include "libxtract.h"
#include <stdio.h>
#include <stdlib.h>

#define BLOCKSIZE 1024
#define SAMPLERATE 44100
#define PERIOD 100
#define MFCC_FREQ_BANDS 13
#define MFCC_FREQ_MIN 20
#define MFCC_FREQ_MAX 20000

using namespace ci;
using namespace ci::app;
using namespace std;


class BasicSampleApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    void test();
};

void BasicSampleApp::setup()
{
   test();
}
void BasicSampleApp::mouseDown( MouseEvent event )
{
    test();
}

void BasicSampleApp::test()
{
    double mean = 0.f;
    double input[BLOCKSIZE];
    double spectrum[BLOCKSIZE];
    double mfccs[MFCC_FREQ_BANDS * sizeof(double)];
    double argd[4];
    int n;
    xtract_mel_filter mel_filters;
    
    // fill the input array with a sawtooth wave
    for(n = 0; n < BLOCKSIZE; ++n)
    {
        input[n] = ((n % PERIOD) / (double)PERIOD) - .5;
    }
    
    xtract[XTRACT_MEAN]( input, BLOCKSIZE, NULL, &mean );
    
    console() << "input_0: " << input[0] << endl;
    
    printf("\nInput mean = %.2f\n\n", mean);
    
    // get the spectrum
    argd[0] = SAMPLERATE / (double)BLOCKSIZE;
    argd[1] = XTRACT_MAGNITUDE_SPECTRUM;
    argd[2] = 0.f; // No DC component
    argd[3] = 0.f; // No Normalisation
    
    xtract_init_fft(BLOCKSIZE, XTRACT_SPECTRUM);
    xtract[XTRACT_SPECTRUM]( input, BLOCKSIZE, argd, spectrum );
    
    
    // print the spectral bins
    printf("\nSpectral bins:\n");
    for(n = 0; n < (BLOCKSIZE >> 1); ++n){
        printf("freq_%d", n + (BLOCKSIZE >> 1) );
        printf(" %.1f\tamp: %.6f\n", spectrum[n + (BLOCKSIZE >> 1)], spectrum[n]);
    }
    printf("\n");
    /*
    // compute the MFCCs
    mel_filters.n_filters = MFCC_FREQ_BANDS;
    mel_filters.filters   = (double**)malloc( MFCC_FREQ_BANDS * sizeof(double*) );
    
    for(n = 0; n < MFCC_FREQ_BANDS; ++n)
    {
        mel_filters.filters[n] = (double*)malloc( BLOCKSIZE * sizeof(double) );
    }
    
    xtract_init_mfcc(BLOCKSIZE >> 1, SAMPLERATE >> 1, XTRACT_EQUAL_GAIN, MFCC_FREQ_MIN, MFCC_FREQ_MAX, mel_filters.n_filters, mel_filters.filters);
    xtract_mfcc( spectrum, BLOCKSIZE >> 1, &mel_filters, mfccs );
    
    // print the MFCCs
    printf("MFCCs:\n");
    for(n = 0; n < MFCC_FREQ_BANDS; ++n)
    {
        printf("band: %d\t", n);
        if(n < 10) {
            printf("\t");
        }
        printf( "coeff: %f\n", mfccs[n] ) ;
    }
    
    // cleanup
    for(n = 0; n < MFCC_FREQ_BANDS; ++n)
    {
        free(mel_filters.filters[n]);
    }
    free(mel_filters.filters);
     */
    
}

void BasicSampleApp::update()
{
}

void BasicSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}


CINDER_APP_BASIC( BasicSampleApp, RendererGl )
