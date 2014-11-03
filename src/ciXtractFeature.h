/*
 *  ciXtractFeature.h
 *
 *  Created by Andrea Cuius
 *  Nocte Studio Ltd. Copyright 2013 . All rights reserved.
 *
 *  www.nocte.co.uk
 *
 */


#ifndef CI_XTRACT_FEATURE
#define CI_XTRACT_FEATURE

#pragma once
#include <boost/algorithm/string.hpp>
#include <map>
#include <vector>
#include "xtract/libxtract.h"

static const std::string xtract_features_names[XTRACT_FEATURES] = {
    "XTRACT_MEAN",
    "XTRACT_VARIANCE",
    "XTRACT_STANDARD_DEVIATION",
    "XTRACT_AVERAGE_DEVIATION",
    "XTRACT_SKEWNESS",
    "XTRACT_KURTOSIS",
    "XTRACT_SPECTRAL_MEAN",
    "XTRACT_SPECTRAL_VARIANCE",
    "XTRACT_SPECTRAL_STANDARD_DEVIATION",
    "XTRACT_SPECTRAL_SKEWNESS",
    "XTRACT_SPECTRAL_KURTOSIS",
    "XTRACT_SPECTRAL_CENTROID",
    "XTRACT_IRREGULARITY_K",
    "XTRACT_IRREGULARITY_J",
    "XTRACT_TRISTIMULUS_1",
    "XTRACT_TRISTIMULUS_2",
    "XTRACT_TRISTIMULUS_3",
    "XTRACT_SMOOTHNESS",
    "XTRACT_SPREAD",
    "XTRACT_ZCR",
    "XTRACT_ROLLOFF",
    "XTRACT_LOUDNESS",
    "XTRACT_FLATNESS",
    "XTRACT_FLATNESS_DB",
    "XTRACT_TONALITY",
    "XTRACT_CREST",
    "XTRACT_NOISINESS",
    "XTRACT_RMS_AMPLITUDE",
    "XTRACT_SPECTRAL_INHARMONICITY",
    "XTRACT_POWER",
    "XTRACT_ODD_EVEN_RATIO",
    "XTRACT_SHARPNESS",
    "XTRACT_SPECTRAL_SLOPE",
    "XTRACT_LOWEST_VALUE",
    "XTRACT_HIGHEST_VALUE",
    "XTRACT_SUM",
    "XTRACT_NONZERO_COUNT",
    "XTRACT_HPS",
    "XTRACT_F0",
    "XTRACT_FAILSAFE_F0",
    "XTRACT_WAVELET_F0",
    "XTRACT_LNORM",
    "XTRACT_FLUX",
    "XTRACT_ATTACK_TIME",
    "XTRACT_DECAY_TIME",
    "XTRACT_DIFFERENCE_VECTOR",
    "XTRACT_AUTOCORRELATION",
    "XTRACT_AMDF",
    "XTRACT_ASDF",
    "XTRACT_BARK_COEFFICIENTS",
    "XTRACT_PEAK_SPECTRUM",
    "XTRACT_SPECTRUM",
    "XTRACT_AUTOCORRELATION_FFT",
    "XTRACT_MFCC",
    "XTRACT_DCT",
    "XTRACT_HARMONIC_SPECTRUM",
    "XTRACT_LPC",
    "XTRACT_LPCC",
    "XTRACT_SUBBANDS",
    "XTRACT_WINDOWE"
};


#define CIXTRACT_PCM_SIZE           1024
#define CIXTRACT_FFT_SIZE           512
#define CIXTRACT_SAMPLERATE         22050 // 44100
#define CIXTRACT_SAMPLERATE_N       CIXTRACT_SAMPLERATE / (double)CIXTRACT_PCM_SIZE

#define CIXTRACT_PERIOD             100
#define CIXTRACT_MFCC_FREQ_BANDS    13
#define CIXTRACT_MFCC_FREQ_MIN      20
#define CIXTRACT_MFCC_FREQ_MAX      20000
#define CIXTRACT_SUBBANDS_N         32

#ifdef _MSC_VER
#ifndef isnan
#define isnan(x) ((x)!=(x))
#endif
#ifndef isinf
#define isinf(x) ((x)!=(x))
#endif
#endif


class ciXtractFeature;
typedef std::shared_ptr<ciXtractFeature>       ciXtractFeatureRef;

class ciXtractSpectrum;

class ciXtract;

typedef std::shared_ptr<double>     DataBuffer;

class FeatureParam;
typedef std::shared_ptr<FeatureParam>   FeatureParamRef;

class FeatureParam {
    
public:
    
    enum ParamType {
        PARAM_READONLY,
        PARAM_EDITABLE
    };
    
    static FeatureParamRef create( std::string name, double initValue, ParamType pType = PARAM_EDITABLE )
    {
        return FeatureParamRef( new FeatureParam( name, initValue, pType ) );
    }
    
    FeatureParam* addOption( std::string label, double value )
    {
        mOptions[label] = value;
        return this;
    }
    
    double getValue()       { return mVal; }
    double *getValuePtr()   { return &mVal; }
    
    std::string getName() { return mName; }
    
private:
    
    FeatureParam( std::string name, double initValue, ParamType pType ) : mName(name), mVal(initValue), mType(pType) {}
    
private:
    
    std::string                     mName;
    double                          mVal;
    std::map<std::string,double>    mOptions;
    ParamType                       mType;
    
    
};



class ciXtractFeature {

public:
    
    friend class ciXtract;
    
    virtual void update( int frameN );
    
public:

    template <class T>
    static ciXtractFeatureRef create( ciXtract *xtract )
    {
        return ciXtractFeatureRef( new T( xtract ) );
    }
    
    
    ///////////////////////////
    
    bool isEnable() { return mIsEnable; }
    
    xtract_features_ getEnum() { return mFeatureEnum; }
    
    double getMin() { return mMin; }
    double getMax() { return mMax; }
    
    std::string getName() { return xtract_features_names[mFeatureEnum]; }
    
    DataBuffer  getData()       { return mData; }
    DataBuffer  getDataRaw()    { return mDataRaw; }
    uint32_t    getDataSize()   { return mDataSize; }
    
    double getValue( int k )    { return mData.get()[ k % mDataSize ]; }
    double getValueRaw( int k ) { return mDataRaw.get()[ k % mDataSize ]; }
    
    std::vector<FeatureParamRef>  getParams() { return mParams; }
    
//    void setParam( std::string name, double val )
//    {
//        mParams[name].val = val;
//    }
    
    
    float   getGain()       { return mGain; }
    float   getOffset()     { return mOffset; }
    float   getDamping()    { return mDamping; }
    bool    isLog()         { return mIsLog; }
    
    void setGain( float val )       { mGain = val; }
    void setOffset( float val )     { mOffset = val; }
    void setDamping( float val )    { mDamping = val; }
    void setLog( bool isLog )       { mIsLog = isLog; }
    
    bool isUpdated( int frameN )    { return mLastUpdateAt == frameN; }
    
protected:
    
    ciXtractFeature(    ciXtract                        *xtract,
                        xtract_features_                featureEnum,
                        uint32_t                        resultsN            = 1,                                        // feature has at least 1 result
                        xtract_features_                inputFeature        = (xtract_features_)(XTRACT_FEATURES),      // XTRACT_FEATURES is for the features that use the PCM as input data
                        std::vector<xtract_features_>   extraDependencies   = std::vector<xtract_features_>() );        // feature can have more dependencies, the input feature if != XTRACT_FEATURES, is automatically added
protected:
    
    bool checkDependencies( int frameN );
    
    void processData();
    
    bool prepareUpdate( int frameN );
    
    void doUpdate( int frameN, const double *inputData, const int inputDataSize, const void *args, double *outputData );
    void doUpdate( int frameN );
    
    void updateWithArgdFeatures( int frameN, std::vector<xtract_features_> features );
    
    void updateWithPcm( int frameN );
    
protected:
    
    ciXtract                        *mXtract;
    
    xtract_features_                mFeatureEnum;
    xtract_features_                mInputFeatureEnum;
    std::vector<xtract_features_>   mDependencies;
    
    DataBuffer                      mDataRaw;           // raw data, no gain, damping etc. - spectrum features also include the frequency bins
    DataBuffer                      mData;              // processed data, spectrum features do NOT include frequency bins
    size_t                          mDataSize;          // results N size, DATA size only, no frequency bins
    
    // mResults parameters, these are used to process the mResultsRaw
    float                           mGain, mOffset, mDamping;
    bool                            mIsLog;
    
    double                          mMin;
    double                          mMax;
    
    bool                            mIsEnable;
    
    std::vector<FeatureParamRef>    mParams;
    double                          mArgd[4];

    int                             mLastUpdateAt;
    
};


// ------------------------------------------------------------------------------------------------ //
// ------------------------------------------------------------------------------------------------ //
// *************************************** VECTOR FEATURES **************************************** //
// ------------------------------------------------------------------------------------------------ //
// ------------------------------------------------------------------------------------------------ //
//
//    xtract_spectrum
//    xtract_autocorrelation_fft
//    xtract_mfcc
//    xtract_dct
//    xtract_autocorrelation
//    xtract_amdf
//    xtract_asdf
//    xtract_bark_coefficients
//    xtract_peak_spectrum
//    xtract_harmonic_spectrum
//    xtract_lpc
//    xtract_lpcc
//    xtract_subbands

// Spectrum
class ciXtractSpectrum : public ciXtractFeature {
    
public:
    ciXtractSpectrum( ciXtract *xtract );
    ~ciXtractSpectrum();
    void update( int frameN  );
};

// Autocorrelation Fft
class ciXtractAutocorrelationFft : public ciXtractFeature {
    
public:
    ciXtractAutocorrelationFft( ciXtract *xtract );
    ~ciXtractAutocorrelationFft() {}
    void update( int frameN );
};

// Mfcc
class ciXtractMfcc : public ciXtractFeature {
    
public:
    ciXtractMfcc( ciXtract *xtract );
    ~ciXtractMfcc();
    void update( int frameN );
private:
    xtract_mel_filter   mMelFilters;
};

//    xtract_dct

// Autocorrelation
class ciXtractAutocorrelation : public ciXtractFeature {
    
public:
    ciXtractAutocorrelation( ciXtract *xtract );
    ~ciXtractAutocorrelation() {}
    void update( int frameN );
};

//    xtract_amdf
//    xtract_asdf

// Bark
class ciXtractBark : public ciXtractFeature {
    
public:
    ciXtractBark( ciXtract *xtract );
    ~ciXtractBark() {}
    void update( int frameN );
private:
    std::shared_ptr<int> mBandLimits;
};

// Peak Spectrum
class ciXtractPeakSpectrum : public ciXtractFeature {
    
public:
    ciXtractPeakSpectrum( ciXtract *xtract );
    ~ciXtractPeakSpectrum()  {}
};

// Harmonic Spectrum
class ciXtractHarmonicSpectrum : public ciXtractFeature {
    
public:
    ciXtractHarmonicSpectrum( ciXtract *xtract );
    ~ciXtractHarmonicSpectrum()  {}
    void update( int frameN );
};

//    xtract_lpc
//    xtract_lpcc
//    xtract_subbands


// ------------------------------------------------------------------------------------------------ //
// ------------------------------------------------------------------------------------------------ //
// *************************************** SCALAR FEATURES **************************************** //
// ------------------------------------------------------------------------------------------------ //
// ------------------------------------------------------------------------------------------------ //
//    xtract_mean
//    xtract_variance
//    xtract_standard_deviation
//    xtract_average_deviation
//    xtract_skewness
//    xtract_kurtosis
//    xtract_spectral_mean
//    xtract_spectral_variance
//    xtract_spectral_standard_deviation
//    xtract_spectral_skewness
//    xtract_spectral_kurtosis
//    xtract_spectral_centroid
//    xtract_irregularity_k
//    xtract_irregularity_j
//    xtract_tristimulus_1
//    xtract_tristimulus_2
//    xtract_spread
//    xtract_zcr
//    xtract_rolloff
//    xtract_loudness
//    xtract_flatness
//    xtract_flatness_db
//    xtract_tonality
//    xtract_noisiness
//    xtract_rms_amplitude
//    xtract_spectral_inharmonicity
//    xtract_crest
//    xtract_power
//    xtract_odd_even_ratio
//    xtract_sharpness
//    xtract_spectral_slope
//    xtract_lowest_value
//    xtract_highest_value
//    xtract_sum
//    xtract_hps
//    xtract_f0
//    xtract_failsafe_f0
//    xtract_wavelet_f0
//    xtract_midicent
//    xtract_nonzero_count
//    xtract_peak

// Mean
class ciXtractMean : public ciXtractFeature {

public:
    ciXtractMean( ciXtract *xtract );
    ~ciXtractMean() {}
    void update( int frameN );
};

// Variance
class ciXtractVariance : public ciXtractFeature {
    
public:
    ciXtractVariance( ciXtract *xtract );
    ~ciXtractVariance() {}
    void update( int frameN );
};

// Standard Deviation
class ciXtractStandardDeviation : public ciXtractFeature {
    
public:
    ciXtractStandardDeviation( ciXtract *xtract );
    ~ciXtractStandardDeviation() {}
    void update( int frameN );
};

// Average Deviation
class ciXtractAverageDeviation : public ciXtractFeature {
    
public:
    ciXtractAverageDeviation( ciXtract *xtract );
    ~ciXtractAverageDeviation() {}
    void update( int frameN );
};

// Skewness
class ciXtractSkewness : public ciXtractFeature {
    
public:
    ciXtractSkewness( ciXtract *xtract );
    ~ciXtractSkewness() {}
    void update( int frameN  );
};

// Kurtosis
class ciXtractKurtosis : public ciXtractFeature {
    
public:
    ciXtractKurtosis( ciXtract *xtract );
    ~ciXtractKurtosis() {}
    void update( int frameN  );
};

// Spectral Mean
class ciXtractSpectralMean : public ciXtractFeature {
    
public:
    ciXtractSpectralMean( ciXtract *xtract );
    ~ciXtractSpectralMean() {}
};

// Spectral Variance
class ciXtractSpectralVariance : public ciXtractFeature {
    
public:
    ciXtractSpectralVariance( ciXtract *xtract );
    ~ciXtractSpectralVariance() {}
    void update( int frameN  );
};

// Spectral Standard Deviation
class ciXtractSpectralStandardDeviation : public ciXtractFeature {
    
public:
    ciXtractSpectralStandardDeviation( ciXtract *xtract );
    ~ciXtractSpectralStandardDeviation() {}
    void update( int frameN  );
};

// Spectral Skewness
class ciXtractSpectralSkewness : public ciXtractFeature {
    
public:
    ciXtractSpectralSkewness( ciXtract *xtract );
    ~ciXtractSpectralSkewness() {}
    void update( int frameN  );
};

// Spectral Kurtosis
class ciXtractSpectralKurtosis : public ciXtractFeature {
    
public:
    ciXtractSpectralKurtosis( ciXtract *xtract );
    ~ciXtractSpectralKurtosis() {}
    void update( int frameN  );

};

// Spectral Centroid
class ciXtractSpectralCentroid : public ciXtractFeature {
    
public:
    ciXtractSpectralCentroid( ciXtract *xtract );
    ~ciXtractSpectralCentroid() {}
};

// Irregularity_k
class ciXtractIrregularityK : public ciXtractFeature {
    
public:
    ciXtractIrregularityK( ciXtract *xtract );
    ~ciXtractIrregularityK() {}
};

// Irregularity_j
class ciXtractIrregularityJ : public ciXtractFeature {
    
public:
    ciXtractIrregularityJ( ciXtract *xtract );
    ~ciXtractIrregularityJ() {}
};

// Tristimulus_1
class ciXtractTristimulus1 : public ciXtractFeature {
    
public:
    ciXtractTristimulus1( ciXtract *xtract );
    ~ciXtractTristimulus1() {}
    void update( int frameN  );
};

// Smoothness
class ciXtractSmoothness : public ciXtractFeature {
    
public:
    ciXtractSmoothness( ciXtract *xtract );
    ~ciXtractSmoothness() {}
};

// Spread
class ciXtractSpread : public ciXtractFeature {
    
public:
    ciXtractSpread( ciXtract *xtract );
    ~ciXtractSpread() {}
    void update( int frameN  );
};

// Zcr
class ciXtractZcr : public ciXtractFeature {
    
public:
    ciXtractZcr( ciXtract *xtract );
    ~ciXtractZcr() {}
};

// Rolloff
//class ciXtractRolloff : public ciXtractFeature {
//    
//public:
//    ciXtractRolloff( ciXtract *xtract );
//    ~ciXtractRolloff() {}
//    void update( int frameN  );
//};

// Loudness
class ciXtractLoudness : public ciXtractFeature {
    
public:
    ciXtractLoudness( ciXtract *xtract );
    ~ciXtractLoudness() {}
};

// Flatness
class ciXtractFlatness : public ciXtractFeature {
    
public:
    ciXtractFlatness( ciXtract *xtract );
    ~ciXtractFlatness() {}
};

// Flatness db
class ciXtractFlatnessDb : public ciXtractFeature {
    
public:
    ciXtractFlatnessDb( ciXtract *xtract );
    ~ciXtractFlatnessDb() {}
};

// Tonality
class ciXtractTonality : public ciXtractFeature {
    
public:
    ciXtractTonality( ciXtract *xtract );
    ~ciXtractTonality() {}
    void update( int frameN  );
};

// Rms Amplitude
class ciXtractRmsAmplitude : public ciXtractFeature {
    
public:
    ciXtractRmsAmplitude( ciXtract *xtract );
    ~ciXtractRmsAmplitude() {}
};

// Spectral Inharmonicity
class ciXtractSpectralInharmonicity : public ciXtractFeature {
    
public:
    ciXtractSpectralInharmonicity( ciXtract *xtract );
    ~ciXtractSpectralInharmonicity() {}
    void update( int frameN  );
};

// Crest
class ciXtractCrest : public ciXtractFeature {
    
public:
    ciXtractCrest( ciXtract *xtract );
    ~ciXtractCrest() {}
    void update( int frameN  );
};

// Power
class ciXtractPower : public ciXtractFeature {
    
public:
    ciXtractPower( ciXtract *xtract );
    ~ciXtractPower() {}
};

// Odd Even Ratio
class ciXtractOddEvenRatio : public ciXtractFeature {
    
public:
    ciXtractOddEvenRatio( ciXtract *xtract );
    ~ciXtractOddEvenRatio() {}
    void update( int frameN  );
};

// Sharpness
class ciXtractSharpness : public ciXtractFeature {
    
public:
    ciXtractSharpness( ciXtract *xtract );
    ~ciXtractSharpness() {}
};

// Spectral Slope
class ciXtractSpectralSlope : public ciXtractFeature {
    
public:
    ciXtractSpectralSlope( ciXtract *xtract );
    ~ciXtractSpectralSlope() {}
};

// Lowest Value
class ciXtractLowestValue : public ciXtractFeature {
    
public:
    ciXtractLowestValue( ciXtract *xtract );
    ~ciXtractLowestValue() {}
};

// Highest Value
class ciXtractHighestValue : public ciXtractFeature {
    
public:
    ciXtractHighestValue( ciXtract *xtract );
    ~ciXtractHighestValue() {}
};

// Sum
class ciXtractSum : public ciXtractFeature {
    
public:
    ciXtractSum( ciXtract *xtract );
    ~ciXtractSum() {}
};

// Hps
class ciXtractHps : public ciXtractFeature {
    
public:
    ciXtractHps( ciXtract *xtract );
    ~ciXtractHps() {}
};

// F0
class ciXtractF0 : public ciXtractFeature {
    
public:
    ciXtractF0( ciXtract *xtract );
    ~ciXtractF0() {}
};

// Failsafe F0
class ciXtractFailsafeF0 : public ciXtractFeature {
    
public:
    ciXtractFailsafeF0( ciXtract *xtract );
    ~ciXtractFailsafeF0() {}
};

// Wavelet F0
class ciXtractWaveletF0 : public ciXtractFeature {
    
public:
    ciXtractWaveletF0( ciXtract *xtract );
    ~ciXtractWaveletF0() {}
};

// Midicent     UTILITY <<<<<<<<<<<<<<<<<<
//class ciXtractMidicent : public ciXtractFeature {
//    
//public:
//    ciXtractMidicent( ciXtract *xtract );
//    ~ciXtractMidicent() {}
//};

// Non Zero Count
class ciXtractNonZeroCount : public ciXtractFeature {
    
public:
    ciXtractNonZeroCount( ciXtract *xtract );
    ~ciXtractNonZeroCount() {}
};

// Peak
//class ciXtractPeak : public ciXtractFeature {
//    
//public:
//    ciXtractPeak( ciXtract *xtract );
//    ~ciXtractPeak() {}
//};


#endif
