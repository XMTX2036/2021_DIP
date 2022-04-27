## Introduction

__Different froms of imaging__: visible, X-ray, Ultrasonic wave Infrared

more than 60% information are obtained by vision

__What's the difference between warping & morphing?__

​	**warping**：图像扭曲(几何变化)
$$
(x,y) = T((u,w))
$$
​	**morphing**：图像变形(几何变形+颜色变化)

​	Two graphs I and J, morphing I and J into a new graph:
$$
M(x, y) = (1-\alpha)I(x,y)+\alpha J(x,y)
$$
__Improve Visual Effect__

​	What's the difference between __super-resolution and common interpolation__？

​	There isn‘t such huge difference between SR and common interpolation. Two of them both use the pixels which have been already known to predict pixel needed. However, SR needs more large amount of data to predict the information(need to use ML).  



__The approach to get the image__

![image-20220108122556347](/Users/xmtx/Library/Application Support/typora-user-images/image-20220108122556347.png)

## Definition of Color Space

Device-dependent:RGB、 CMY(cyan, mayenta, yellow)/CMYK(CMY and black)、 HSV(H:hue色调, S:saturation饱和度, V:value亮度)

Device-indepedent：CIE XYZ, CIE $L^*a^*b^*$(亮度、绿至红的分量，蓝至黄的分量), CIE YUV($YC_rC_b$, Y->value, U,V->hue)

CIE表色系(RGB)

## JPEG Format

Joint Photographic Experts

.JPEG, .JFIF, .JPG,  JPE

__Compression Strategy:__ According to the requirement of compression ratio, remove information from high frequency to low frequency.

__Advantages:__

high frequency removal leads to high compression ratio;

low frequency information preserves the principle structure and color distribution of object, which is the key factors of an image.

suitable for internet based visual media. 

**Chains:Run length encoding(RLE)**

```C
"aaaaaaabbccccdefffffffgg"
"a7b2c4d1e1f7g2"
```

**Compression**

无压缩bmp、有压缩（有损压缩 JPEG& 无损压缩PNG）

有损压缩：JPEG，GIF，MP3，MP4，MKV，OGG etc

无损压缩：RAW，BMP，PNG，WAV，FLAC，ALAC etc

## Binary Image

**Acquisition of Binary Image:**

Set a threshold, if I(x, y) >threshold, I(x,y) = 255, else I(x, y) = 0.

Then --> How to get a good threshold?

__Ostu Algorithm:__

```C++
int Ostu(BYTE *graydata, double *variance)
{
	int res, size = bmpInfo.biSizeImage;
    int max_value = *max_element(graydata, graydata+size);
    int min_value = *min_element(graydata, graydata+size);
    for(int L=min_value+1; L<=max_value; L++){
        int cnt_fgrd = 0, cnt_bgrd = 0;
        double mean_fgrd = 0, mean_bgrd = 0, p_fgrd, p_bgrd;        
        for(int i=0; i<size; i++){
            if((int)graydata[i] < L){
                cnt_fgrd++;
                mean_fgrd += graydata[i];
            }else{
                cnt_bgrd++;
                mean_bgrd += graydata[i];
            }
        }
        // cout << cnt_fgrd << ";"<< cnt_bgrd << endl;
        mean_fgrd /= (1.0*cnt_fgrd);
        mean_bgrd /= (1.0*cnt_bgrd);
        // if(L<10) cout << "mean value: " << mean_fgrd << ";" << mean_bgrd << endl;
        p_bgrd = cnt_bgrd*1.0 / size;
        p_fgrd = cnt_fgrd*1.0 / size;
        variance[L] = p_fgrd*p_bgrd*(mean_bgrd-mean_fgrd)*(mean_bgrd-mean_fgrd);
        // cout << "probability" << p_fgrd << ";" << p_bgrd << endl;
        // cout << distance(variance, max_element(variance, variance+256)) << endl;
    }
    res = distance(variance, max_element(variance, variance+256));
	return res;
}
```

Global thresholding is __NOT Geiliable__!

Solution:Local adaptive operation

Original image --> Global binarization --> Local adaptive binarization

**Advantages**:

Less memory

More efficient

CAN sometimes be applied on grayscale image

More cheap

**Disadvantages**:

Application field is limited

CANNOT be applied on 3-D data

Less expressive force, FAIL to convey the visual details

CANNOT control contrast

## Morphology

**Dilation:** adopts the connected background pixels into the foreground, which extends its boundary and fill the holes in the foreground.

**Erosion:** remove boundary, remove unwanted small objects

**Opening:** erosion then dilation

**Closing:** dilation then erosion

## Enhancement & Histogram

Visible threshold:$\frac{\Delta I}{I} = K_{weber}$
$$
\frac{I_{max}}{I_{min}}=(1+K_{weber})^{255}
$$
**Gamma Correction:**
$$
I_0 \quad \to \quad (aI_0)^\gamma  
$$
With larger $\gamma$, the contrast is more larger; with smaller $\gamma$, there would be more visual details 

![截屏2022-01-08 下午11.41.58](/Users/xmtx/Library/Application Support/typora-user-images/截屏2022-01-08 下午11.41.58.png)

**Visible Enhancement:Logarithmic Operation**
$$
L_d = \frac{log(L_w+1)}{log(L_{max}+1)}
$$

## Grayscale Image and Histogram

Color information can be inferred from the Histogram of a image, however, the structure information cannot be inferred. 

**Histogram Equalization:**

**Q:**Why after the histogram equalization the probabilities of different gray levels are not the same?

**A:**$s_k$ is not exactly equivalent to one of the eight gray levels.Hence, it is included by a nearby gray level. So several neighboring $s_k$ may falls into the same gray level. After discrete histogram equalizaton, different gray levels' probability may be different.

**Histogram Fitting:**

......

## Geometric Transformation

**Translation**

**Mirror**

**Rotation**:"Hole" problem

Fill the hole by interpolation -->**Nearest Neighbor Based Interpolation**

![image-20220109001415457](/Users/xmtx/Library/Application Support/typora-user-images/image-20220109001415457.png)

## Interpolation

**Nearest Neighbor Based Interpolation:**

**Linear Interpolation:**

Bilinear Equation:
$$
g(x,y) = ax+by+cxy+d
$$
**Morph**

Change image A to image B
$$
I_i=I_{begin}+i*\frac{I_{end}-I_{begin}}{n}
$$
表情比例图？

## Convolution & Spatial Filtering

**Continuous 1D Convolution:**
$$
g(x) = f(x)*h(x) = \int_{-\infty}^{+\infty}f(t)h(x-t){\rm d}t
$$
**Discrete 1D Convolution:**
$$
g(x)=f(x)*h(x) = \frac{1}{M}\Sigma_{t=0}^{M-1}f(t)h(x-t)
$$
**Spatial filtering for smoothing**
$$
\frac{1}{9}
\left[
\begin{matrix}
1 & 1 & 1 \\
1 & 1 & 1 \\
1 & 1 & 1 
\end{matrix} \right]\tag{2}
$$
**Statistical sorting filter —— Median filter**

Laplacian Operator:
$$
\nabla^2f=\frac{\partial^2f}{\partial x^2}+\frac{\partial^2f}{\partial y^2}
$$

$$
\frac{\partial^2f}{\partial x^2}=f(x+1,y)+f(x-1,y)-2f(x,y)
$$

$$
\frac{\partial^2f}{\partial y^2}=f(x,y+1)+f(x,y-1)-2f(x,y)
$$

**Bilateral Filter**

An image has two main characteristics

**The space domain S & The intensity domain R**

![截屏2022-01-09 上午10.31.23](/Users/xmtx/Library/Application Support/typora-user-images/截屏2022-01-09 上午10.31.23.png)

![截屏2022-01-09 上午10.32.10](/Users/xmtx/Library/Application Support/typora-user-images/截屏2022-01-09 上午10.32.10.png)

## Fourier Transform

Fourier Analysis = Frequency domain Analysis

FFT & DFT