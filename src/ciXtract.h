/*
 *  ciXtract.h
 *
 *  Created by Andrea Cuius
 *  Nocte Studio Ltd. Copyright 2013 . All rights reserved.
 *
 *  www.nocte.co.uk
 *
 */


#ifndef CI_XTRACT
#define CI_XTRACT

#pragma once
#include "cinder/app/App.h"
#include "ciXtractFeature.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define CI_XTRACT_CALIBRATION_DURATION 3.0f

class ciXtract;
typedef std::shared_ptr<ciXtract>       ciXtractRef;


class ciXtract {

public:
    
	static ciXtractRef create() { return ciXtractRef( new ciXtract() ); }
    
    ~ciXtract();
    
    //! process the data
    void update( const float *pcmData );
    
    //! enable a feature
    void enableFeature( xtract_features_ feature );
    
    //! disable a feature
    void disableFeature( xtract_features_ feature );
    
    //! toggle a feature
    void toggleFeature( xtract_features_ feature );
    
    //! return all the available features
    std::vector<ciXtractFeatureRef> getFeatures() { return mFeatures; };
    
    //! get a specific feature
    ciXtractFeatureRef getFeature( xtract_features_ feature );
    
    //! get raw feature results
    std::shared_ptr<double> getFeatureResultsRaw( xtract_features_ feature )
    {
        return getFeature(feature)->getResultsRaw();
    }

    //! get feature results
    std::shared_ptr<double> getFeatureResults( xtract_features_ feature )
    {
        return getFeature(feature)->getResults();
    }

    // The calibration is very rough and it doesn't work for all the features

    //! return true if there is any feature calibration in progress
    bool isCalibrating() { return !mCalibrationFeatures.empty(); }

    //! calibrate all the features at once
    void calibrateFeatures();
    
    //! calibrate a feature
    void calibrateFeature( ciXtractFeatureRef feature );
    
    //! calibrate a feature
    void calibrateFeature( xtract_features_ featureEnum );
    
    //! get PCM data
    std::shared_ptr<double> getPcmData() { return mPcmData; }
    
    //! list all the available features, the enumarators can be used to identify or toggle the features
    void listFeatures();

private:
    
    ciXtract();

    void initFeatures();
    
    bool featureDependsOn( xtract_features_ this_feature, xtract_features_ test_feature );
    
    void updateCalibration();
    
    
private:
    
    struct ciXtractFeatureCalibration
    {
        ciXtractFeatureRef  feature;
        double              StartedAt;
    };
    
    std::vector<ciXtractFeatureRef>             mFeatures;
    std::vector<ciXtractFeatureCalibration>     mCalibrationFeatures;
    
    std::shared_ptr<double>                     mPcmData;
    
};

#endif
