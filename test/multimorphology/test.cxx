/************************************************************************/
/*                                                                      */
/*                 Copyright 2004 by Ullrich Koethe                     */
/*       Cognitive Systems Group, University of Hamburg, Germany        */
/*                                                                      */
/*    This file is part of the VIGRA computer vision library.           */
/*    The VIGRA Website is                                              */
/*        http://kogs-www.informatik.uni-hamburg.de/~koethe/vigra/      */
/*    Please direct questions, bug reports, and contributions to        */
/*        ullrich.koethe@iwr.uni-heidelberg.de    or                    */
/*        vigra@informatik.uni-hamburg.de                               */
/*                                                                      */
/*    Permission is hereby granted, free of charge, to any person       */
/*    obtaining a copy of this software and associated documentation    */
/*    files (the "Software"), to deal in the Software without           */
/*    restriction, including without limitation the rights to use,      */
/*    copy, modify, merge, publish, distribute, sublicense, and/or      */
/*    sell copies of the Software, and to permit persons to whom the    */
/*    Software is furnished to do so, subject to the following          */
/*    conditions:                                                       */
/*                                                                      */
/*    The above copyright notice and this permission notice shall be    */
/*    included in all copies or substantial portions of the             */
/*    Software.                                                         */
/*                                                                      */
/*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND    */
/*    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES   */
/*    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND          */
/*    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT       */
/*    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,      */
/*    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      */
/*    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR     */
/*    OTHER DEALINGS IN THE SOFTWARE.                                   */                
/*                                                                      */
/************************************************************************/

#include <iostream>
#include "unittest.hxx"
#include "vigra/stdimage.hxx"
#include "vigra/multi_morphology.hxx"
#include "vigra/linear_algebra.hxx"
#include "vigra/matrix.hxx"

using namespace vigra;

struct MultiMorphologyTest
{
    typedef vigra::MultiArray<3,int> IntVolume;
    typedef vigra::MultiArray<2,int> IntImage;

    MultiMorphologyTest()
    : img(IntImage::difference_type(7,7)), img2(IntImage::difference_type(7,7)), vol(IntVolume::difference_type(5,5,5)), lin(IntImage::difference_type(7,1))
    {
        static const unsigned char in[] = {
            0, 1, 1, 1, 1, 1, 0,
            0, 1, 1, 1, 1, 1, 0,
            0, 1, 1, 1, 1, 1, 0,
            0, 1, 1, 1, 1, 1, 0,
            0, 1, 1, 1, 1, 1, 0,
            0, 1, 1, 1, 1, 1, 0,
            0, 1, 1, 1, 1, 1, 0};
        
        const unsigned char *i=in;
        for(IntImage::iterator iter=img.begin(); iter!=img.end(); ++iter, ++i){
            *iter=*i;
        }
        static const unsigned char in1d[] = {0, 1, 1, 1, 1, 1, 0};
        i=in1d;
        for(IntImage::iterator iter=lin.begin(); iter!=lin.end(); ++iter, ++i){
            *iter=*i;
        }

        static const unsigned char in1[] = {
            0, 1, 2, 3, 4, 5, 6,
            0, 1, 2, 3, 4, 5, 6,
            0, 1, 2, 3, 4, 5, 6,
            0, 1, 2, 3, 4, 5, 6,
            0, 1, 2, 3, 4, 5, 6,
            0, 1, 2, 3, 4, 5, 6,
            0, 1, 2, 3, 4, 5, 6};
        
        i=in1;
        for(IntImage::iterator iter=img2.begin(); iter!=img2.end(); ++iter, ++i){
            *iter=*i;
        }

        static const unsigned char in2[] = { 0, 0, 0, 0, 0, 
                                             0, 0, 0, 0, 0,  
                                             0, 0, 0, 0, 0,  
                                             0, 0, 0, 0, 0,  
                                             0, 0, 0, 0, 0,

                                             0, 0, 0, 0, 0,  
                                             0, 1, 1, 1, 0,  
                                             0, 1, 1, 1, 0,  
                                             0, 1, 1, 1, 0,  
                                             0, 0, 0, 0, 0,

                                             0, 0, 0, 0, 0,  
                                             0, 1, 1, 1, 0,  
                                             0, 1, 1, 1, 0,  
                                             0, 1, 1, 1, 0,  
                                             0, 0, 0, 0, 0,

                                             0, 0, 0, 0, 0,  
                                             0, 1, 1, 1, 0,  
                                             0, 1, 1, 1, 0,  
                                             0, 1, 1, 1, 0,  
                                             0, 0, 0, 0, 0,

                                             0, 0, 0, 0, 0, 
                                             0, 0, 0, 0, 0,  
                                             0, 0, 0, 0, 0,  
                                             0, 0, 0, 0, 0,  
                                             0, 0, 0, 0, 0};

        i=in2;
        for(IntVolume::iterator iter=vol.begin(); iter!=vol.end(); ++iter, ++i){
            *iter=*i;
        }
    }
    
    void binaryErosionTest()
    {
        IntImage res(img);
        int foreground = NumericTraits<int>::one();
        
        static const int desired[] = {
                   0, 0, 0, foreground, 0, 0, 0,
                   0, 0, 0, foreground, 0, 0, 0,
                   0, 0, 0, foreground, 0, 0, 0,
                   0, 0, 0, foreground, 0, 0, 0,
                   0, 0, 0, foreground, 0, 0, 0,
                   0, 0, 0, foreground, 0, 0, 0,
                   0, 0, 0, foreground, 0, 0, 0};
        
        multiBinaryErosion(srcMultiArrayRange(img), destMultiArray(res), 2);
        shouldEqualSequence(res.begin(), res.end(), desired);
    }

    void binaryErosionTest2()
    {
        IntImage res(img2);
        int foreground = NumericTraits<int>::one();
        
        static const int desired[] = {
                   0, 0, 0, foreground, foreground, foreground, foreground,
                   0, 0, 0, foreground, foreground, foreground, foreground,
                   0, 0, 0, foreground, foreground, foreground, foreground,
                   0, 0, 0, foreground, foreground, foreground, foreground,
                   0, 0, 0, foreground, foreground, foreground, foreground,
                   0, 0, 0, foreground, foreground, foreground, foreground,
                   0, 0, 0, foreground, foreground, foreground, foreground};

        multiBinaryErosion(srcMultiArrayRange(img2), destMultiArray(res), 2);
        shouldEqualSequence(res.begin(), res.end(), desired);
    }

    void binaryErosionTest1D()
    {
        IntImage res(lin);
        int foreground = NumericTraits<int>::one();
        
        static const int desired[] = {0, 0, 0, foreground, 0, 0, 0};
        multiBinaryErosion(srcMultiArrayRange(lin), destMultiArray(res), 2);
        shouldEqualSequence(res.begin(), res.end(), desired);
    }

    void binaryErosionTest3D()
    {
        IntVolume res(vol);
        int f = NumericTraits<int>::one();
        
        static const int desired[] = {  0, 0, 0, 0, 0, 
                                        0, 0, 0, 0, 0,  
                                        0, 0, 0, 0, 0,  
                                        0, 0, 0, 0, 0,  
                                        0, 0, 0, 0, 0,

                                        0, 0, 0, 0, 0, 
                                        0, 0, 0, 0, 0,  
                                        0, 0, 0, 0, 0,  
                                        0, 0, 0, 0, 0,  
                                        0, 0, 0, 0, 0,

                                        0, 0, 0, 0, 0, 
                                        0, 0, 0, 0, 0,  
                                        0, 0, f, 0, 0,  
                                        0, 0, 0, 0, 0,  
                                        0, 0, 0, 0, 0,

                                        0, 0, 0, 0, 0, 
                                        0, 0, 0, 0, 0,  
                                        0, 0, 0, 0, 0,  
                                        0, 0, 0, 0, 0,  
                                        0, 0, 0, 0, 0,

                                        0, 0, 0, 0, 0, 
                                        0, 0, 0, 0, 0,  
                                        0, 0, 0, 0, 0,  
                                        0, 0, 0, 0, 0,  
                                        0, 0, 0, 0, 0};

        multiBinaryErosion(srcMultiArrayRange(vol), destMultiArray(res), 1);
        shouldEqualSequence(res.begin(), res.end(), desired);
    }
    
    void grayErosionTest2D()
    {
        typedef vigra::MultiArray<2,float> FloatImage;
        FloatImage in(img), res(img), res_cmp(img);
        
        //erosion on original image
        multiGrayscaleErosion(srcMultiArrayRange(in), destMultiArray(res), 1);
        
        //create comparable result = result+2 for every pixel
        for(FloatImage::iterator iter=res.begin(); iter!=res.end(); ++iter){
            *iter+=2.9f;
        }
        
        //create compare image = img+2 for every pixel
        for(FloatImage::iterator iter=in.begin(); iter!=in.end(); ++iter){
            *iter+=2.9f;
        }
        //erosion on compare image (image+2)
        multiGrayscaleErosion(srcMultiArrayRange(in), destMultiArray(res_cmp), 1);
        
        shouldEqualSequence(res.begin(), res.end(), res_cmp.begin());
    }

    void grayDilationTest2D()
    {
        typedef vigra::MultiArray<2,float> FloatImage;
        FloatImage in(img), res(img), res_cmp(img);

        //dilation on original image
        multiGrayscaleDilation(srcMultiArrayRange(in), destMultiArray(res), 1);

        //create comparable result = result+2 for every pixel
        for(FloatImage::iterator iter=res.begin(); iter!=res.end(); ++iter){
            *iter+=2.9f;
        }

        //create compare image = img+2 for every pixel
        for(FloatImage::iterator iter=in.begin(); iter!=in.end(); ++iter){
            *iter+=2.9f;
        }
        //dilation on compare image (image+2)
        multiGrayscaleDilation(srcMultiArrayRange(in), destMultiArray(res_cmp), 1);

        shouldEqualSequence(res.begin(), res.end(), res_cmp.begin());
    }

    void grayErosionAndDilationTest2D()
    {
        typedef vigra::MultiArray<2, float> FloatImage;
        FloatImage in(img), di_res(img), er_res(img);

        //erosion on original image
        multiGrayscaleErosion(srcMultiArrayRange(in),destMultiArray(er_res),1);
        //dilation on original inverted image
        for(FloatImage::iterator iter=in.begin(); iter!=in.end(); ++iter){
            *iter*=-1.0f;
        }
        multiGrayscaleDilation(srcMultiArrayRange(in),destMultiArray(di_res),1);
        //Invert dilation res
        for(FloatImage::iterator iter=di_res.begin(); iter!=di_res.end(); ++iter){
            *iter*=-1.0f;
        }

        shouldEqualSequence(di_res.begin(),di_res.end(), er_res.begin());
    }

    void grayClosingTest2D()
    {
        typedef vigra::MultiArray<2,UInt8> UInt8Image;
        UInt8Image in(img), tmp(img), res(img);

        //erosion on original image
        multiGrayscaleErosion(srcMultiArrayRange(in), destMultiArray(tmp),2);
        multiGrayscaleDilation(srcMultiArrayRange(tmp), destMultiArray(res),2);
    }
    
    IntImage img, img2, lin;
    IntVolume vol;
};

        
struct MorphologyTestSuite
: public vigra::test_suite
{
    MorphologyTestSuite()
    : vigra::test_suite("MorphologyTestSuite")
    {
        add( testCase( &MultiMorphologyTest::binaryErosionTest));
        add( testCase( &MultiMorphologyTest::binaryErosionTest2));
        add( testCase( &MultiMorphologyTest::binaryErosionTest1D));
        add( testCase( &MultiMorphologyTest::binaryErosionTest3D));
        add( testCase( &MultiMorphologyTest::grayErosionTest2D));
        add( testCase( &MultiMorphologyTest::grayDilationTest2D));
        add( testCase( &MultiMorphologyTest::grayErosionAndDilationTest2D));
        add( testCase( &MultiMorphologyTest::grayClosingTest2D));
    }
};

int main(int argc, char ** argv)
{
    MorphologyTestSuite test;

    int failed = test.run(vigra::testsToBeExecuted(argc, argv));

    std::cout << test.report() << std::endl;
    return (failed != 0);
}

