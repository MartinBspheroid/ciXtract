/*
 *  ciLibXtract.h
 *
 *  Created by Andrea Cuius
 *  Nocte Studio Ltd. Copyright 2013 . All rights reserved.
 *
 *  www.nocte.co.uk
 *
 */


#ifndef CILIBXTRACT
#define CILIBXTRACT

#pragma once

#include "libxtract.h"
#include "cinder/audio/Input.h"

#include "cinder/gl/TextureFont.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define PCM_SIZE            2048
#define FFT_SIZE            512
#define SAMPLERATE          44100

#define PERIOD              100
#define MFCC_FREQ_BANDS     13
#define MFCC_FREQ_MIN       20
#define MFCC_FREQ_MAX       20000
#define SUBBANDS_N          32


class ciLibXtract;
typedef std::shared_ptr<ciLibXtract>    ciLibXtractRef;


class ciLibXtract {

public:
    
    enum FeatureType {
        VECTOR_FEATURE,
        SCALAR_FEATURE
    };
    
    typedef struct {
        xtract_features_                feature;
        std::string                     name;
        std::function<void()>           cb;
        bool                            enable;
        FeatureType                     type;
        std::vector<xtract_features_>   dependencies;
        double                          min;
        double                          max;
        uint32_t                        buffSize;
    } FeatureCallback;
    
    
public:
    
    static ciLibXtractRef create( audio::Input source ) { return ciLibXtractRef( new ciLibXtract( source ) ); }
    
    ~ciLibXtract();
    
    void update();
    
    void enableFeature( xtract_features_ feature );
    
    void disableFeature( xtract_features_ feature );
    
    void toggleFeature( xtract_features_ feature );
    
    std::shared_ptr<double> getVectorFeature( xtract_features_ feature );
    
    double getScalarFeature( xtract_features_ feature )
    {
        return mScalarValues[feature];
    }

    double* getScalarFeaturePtr( xtract_features_ feature )
    {
        return &mScalarValues[feature];
    }
    
    FeatureCallback* findFeatureCbRef( xtract_features_ feature );
    
    double getFeatureMin( xtract_features_ feature ) { return mAutoCalibration[feature].min; }
    double getFeatureMax( xtract_features_ feature ) { return mAutoCalibration[feature].max; }

    void autoCalibrate( bool run );
    bool isCalibrating() { return mRunCalibration; }
    void toggleCalibration() { autoCalibrate( !mRunCalibration ); }
    
private:
    
    void updateCalibration();
    
    struct CalibrationValues {
        double min;
        double max;
    };
    
    CalibrationValues       mAutoCalibration[XTRACT_FEATURES];
    bool                    mRunCalibration;

    
private:
    
    ciLibXtract( audio::Input source );
    
    void init();

    void initFft();
    void initMfccs();
    void initBarks();
    void initParams();
    void initCallbacks();
    
    void updateCallbacks();
    
    void xtractFeature( xtract_features_ feature );
    
    bool featureDependsOn( xtract_features_ this_feature, xtract_features_ test_feature );
    
    
    // ------------------------------------ //
    //              Callbacks               //
    // ------------------------------------ //
    
private:
    
    void updateMean();
    void updateVariance();
    void updateStandardDeviation();
    void updateAverageDeviation();
    void updateSkewness();
    void updateKurtosis();
    void updateSpectralMean();
    void updateSpectralVariance();
    void updateSpectralStandardDeviation();
    void updateSpectralSkewness();
    void updateSpectralKurtosis();
    void updateSpectralCentroid();
    void updateIrregularityK();
    void updateIrregularityJ();
    void updateTristimulus1();

    void updateSmoothness();
    void updateSpread();
    void updateZcr();
    void updateRollOff();
    void updateLoudness();
    void updateFlatness();
    void updateFlatnessDb();
    
    void updateTonality();
//    void updateCrest();
//    void updateNoisiness();
    void updateRmsAmplitude();
    void updateSpectralInharmonicity();
    void updatePower();
    void updateOddEvenRatio();
    void updateSharpness ();
    
    
    
    
    void updateSpectralSlope();
    void updateLowestValue();
    void updateHighestValue();
    void updateSum();
    void updateNonZeroCount();
    void updateF0();
    
//    void updateFailsafeF0();
//    void updateLnorm();
//    void updateFlux();
//    void updateAttackTime();
//    void updateDecayTime();
//    void updateDifferenceVector();
//    void updateHps();
    
    void updateSpectrum();
    void updateAutoCorrelationFft();
    void updateAutoCorrelation();
    void updateMfcc();
    void updateBarkCoefficients();
    void updatePeakSpectrum();
    void updateHarmonicSpectrum();
    void updateSubBands();
    
    //     void updateDct();
    //     void updateAmdf();
    //     void updateAsdf();
    //    void updateLpc();
//    void updateLpcc();
//    
//    void updateWindowed();
    
    
    
    // TEMPORARY PUBLIC !!! <<<<<<<<<<<<<<
public:
//    std::map<xtract_features_,FeatureCallback>  mCallbacks;
    std::vector<FeatureCallback>  mCallbacks;


private:

    
    ci::audio::Input                            mInputSource;
	audio::PcmBuffer32fRef                      mPcmBuffer;
    std::shared_ptr<double>                     mPcmData;
    std::shared_ptr<double>                     mSpectrum;
    std::shared_ptr<double>                     mPeakSpectrum;
    std::shared_ptr<double>                     mHarmonicSpectrum;
    std::shared_ptr<double>                     mBarks;
    std::shared_ptr<double>                     mAutocorrelation;
    std::shared_ptr<double>                     mAutocorrelationFft;
    std::shared_ptr<double>                     mSubBands;
    
    std::shared_ptr<int>                        mBarkBandLimits;

    std::shared_ptr<double>                     mMfccs;
    xtract_mel_filter                           mMelFilters;
    
    double  mScalarValues[XTRACT_FEATURES];     // the array is bigger but at least you can refer to each item using xtract_features_
    
    std::map<std::string,double>                mParams;
    
    double                                      _argd[4];
    
    ci::gl::TextureFontRef                      mFontSmall;
    
    
};

#endif
