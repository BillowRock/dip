#include "imageprocess.h"


//对边缘区域进行处理，因为其像素邻域不完全在图像内部，通过镜像边缘区域以避免领域直方图中出现峰值

void paddingZeros(const float *r, const float *g, const float *b,
                   const int width, const int height,
                   const int half_pad_width, const int half_pad_height,
                   float *nr, float *ng, float *nb)
{
    int nw = width + 2*half_pad_width;
    int nh = height + 2*half_pad_height;
// 初始化内存区域

    memset(nr, 0, nh*nw*sizeof(float));
    memset(ng, 0, nh*nw*sizeof(float));
    memset(nb, 0, nh*nw*sizeof(float));

    for (int j=half_pad_height; j<nh-half_pad_height; j++) {
        memcpy(nr+j*nw+half_pad_width, r+(j-half_pad_height)*width, width*sizeof(float));
        memcpy(ng+j*nw+half_pad_width, g+(j-half_pad_height)*width, width*sizeof(float));
        memcpy(nb+j*nw+half_pad_width, b+(j-half_pad_height)*width, width*sizeof(float));
    }
}

void splitImageChannel(QImage &image, float *r, float *g, float *b)
{
    // 将图像R\G\B数据分别存贮至r\g\b

    int width = image.width();
    int height = image.height();
    int count = 0;
    // 在取图像RGB值时，对整个图像像素点进行了遍历，对于大尺寸图片速度明显过慢，是否应改为直接移动指针对像素进行读取以加快速度

    for (int j=0; j<height; j++) {
        for (int i=0; i<width; i++) {
            QRgb pixel = image.pixel(i, j);
            r[count] = qRed(pixel);
            g[count] = qGreen(pixel);
            b[count] = qBlue(pixel);
            count++;
        }
    }
}

void splitImageChannel(QImage &image, float *rgb)
{
    int width = image.width();
    int height = image.height();
    int count = 0;
    for (int j=0; j<height; j++) {
        for (int i=0; i<width; i++) {
            QRgb pixel = image.pixel(i, j);
            rgb[j*3*width+3*i] = (float)qRed(pixel);
            rgb[j*3*width+3*i+1] = (float)qGreen(pixel);
            rgb[j*3*width+3*i+2] = (float)qBlue(pixel);
            count++;
        }
    }
}

void splitImageChannel(QImage &image, uchar *r, uchar *g, uchar *b)
{
    int width = image.width();
    int height = image.height();
    int count = 0;
    for (int j=0; j<height; j++) {
        for (int i=0; i<width; i++) {
            QRgb pixel = image.pixel(i, j);
            r[count] = qRed(pixel);
            g[count] = qGreen(pixel);
            b[count] = qBlue(pixel);
            count++;
        }
    }
}

void splitImageChannel(QImage &image, uchar *rgb)
{
    int width = image.width();
    int height = image.height();
    int count = 0;
    for (int j=0; j<height; j++) {
        for (int i=0; i<width; i++) {
            QRgb pixel = image.pixel(i, j);
            rgb[j*3*width+3*i] = (uchar)qRed(pixel);
            rgb[j*3*width+3*i+1] = (uchar)qGreen(pixel);
            rgb[j*3*width+3*i+2] = (uchar)qBlue(pixel);
            count++;
        }
    }
}
// 根据RGB数据生成图片

void concatenateImageChannel(float *r, float *g, float *b, int w, int h, QImage &image)
{
    image = QImage(w, h, QImage::Format_RGB888);
    for (int j=0; j<h; j++) {
        for (int i=0; i<w; i++) {
            QRgb pixel = qRgb((int)r, (int)g, (int)b);
            image.setPixel(i, j, pixel);
        }
    }
}

void concatenateImageChannel(float *rgb, int w, int h, QImage &image)
{
    image = QImage(w, h, QImage::Format_RGB888);
    for (int j=0; j<h; j++) {
        for (int i=0; i<w; i++) {
            int r = (int)rgb[j*3*w+3*i];
            int g = (int)rgb[j*3*w+3*i+1];
            int b = (int)rgb[j*3*w+3*i+2];
            QRgb pixel = qRgb(r, g, b);
            image.setPixel(i, j, pixel);
        }
    }
}

void concatenateImageChannel(uchar *r, uchar *g, uchar *b, int w, int h, QImage &image)
{
    image = QImage(w, h, QImage::Format_RGB888);
    for (int j=0; j<h; j++) {
        for (int i=0; i<w; i++) {
            QRgb pixel = qRgb((int)r, (int)g, (int)b);
            image.setPixel(i, j, pixel);
        }
    }
}

void concatenateImageChannel(uchar *rgb, int w, int h, QImage &image)
{
    image = QImage(w, h, QImage::Format_RGB888);
    for (int j=0; j<h; j++) {
        for (int i=0; i<w; i++) {
            int r = (int)rgb[j*3*w+3*i];
            int g = (int)rgb[j*3*w+3*i+1];
            int b = (int)rgb[j*3*w+3*i+2];
            QRgb pixel = qRgb(r, g, b);
            image.setPixel(i, j, pixel);
        }
    }
}
//RGB转YCrCb空间，主要为了利用Y通道

void rgb2ycrcb(uchar *r, uchar *g, uchar *b, int size, float *y, float *cr, float *cb)
{
    for (int i=0; i<size; i++)
    {
        y[i] = 0.256789 * r[i] + 0.504129 * g[i] + 0.097906 * b[i] + 16;
        cb[i] = -0.148223 * r[i] - 0.290992 * g[i] + 0.439215 * b[i] + 128;
        cr[i] = 0.439215 * r[i] - 0.367789 * g[i] - 0.071426 * b[i] + 128;
    }
}
//YCrCb空间处理后经此函数转回RGB

void ycrcb2rgb(float *y, float *cr, float *cb, int size, uchar *r, uchar *g, uchar *b)
{
    for (int i=0; i<size; i++)
    {
        r[i] = 1.164383 * (y[i]-16) + 1.596027 * (cr[i]-128);
        g[i] = 1.164383 * (y[i]-16) - 0.391762 * (cb[i]-128)- 0.812969 * (cr[i]-128);
        b[i] = 1.164383 * (y[i]-16) + 2.017230 * (cb[i]-128);
    }
}

void qimage2ycrcb(QImage image, float *y, float *cr, float *cb)
{
    int pixel_num = image.width()*image.height();
    // obtain image channels
    uchar *channels = new uchar[pixel_num*3];
    uchar *r = channels;
    uchar *g = channels+pixel_num;
    uchar *b = channels+2*pixel_num;
    splitImageChannel(image, r, g, b);

    // rgb to ycrcb
    rgb2ycrcb(r, g, b, pixel_num, y, cr, cb);

    delete [] channels;
}

void ycrcb2qimage(float *y, float *cr, float *cb, int width, int height, QImage &image)
{
    int pixel_num = width*height;

    uchar *channels = new uchar[pixel_num*3];
    uchar *r = channels;
    uchar *g = channels+pixel_num;
    uchar *b = channels+2*pixel_num;

    // ycrcb to rgb
    ycrcb2rgb(y, cr, cb, pixel_num, r, g, b);

    // update image
    int count = 0;
    image = QImage(width, height, QImage::Format_RGB888);
    for (int j=0; j<height; j++) {
        for (int i=0; i<width; i++) {
            int nr = r[count];
            int ng = g[count];
            int nb = b[count];
            count++;
            image.setPixel(i, j, qRgb(nr,ng,nb));
        }
    }
    delete [] channels;
}

//根据图像RGB值计算各直方图

QImage calculateHistogram(QImage &image, ImageChannel channel)
{
    // obtain gray image
    QImage grayImage = image.convertToFormat(QImage::Format_Grayscale8);
    int width = grayImage.width();
    int height = grayImage.height();
    uchar *r = new uchar[width*height];
    uchar *g = new uchar[width*height];
    uchar *b = new uchar[width*height];

    // seperate RGB channel
    splitImageChannel(image, r, g, b);

    QRgb hist_ior;

    uchar *bits = nullptr;
    switch (channel) {
        case ImageChannel::Y://先生成灰度图，计算灰度直方图
            hist_ior = qRgba(128, 128, 128, 255);
            bits = grayImage.bits();
            break;
        case ImageChannel::R://灰度直方图(仅按R)
            hist_ior = qRgba(255, 0, 0, 255);
            bits = r;
            break;
        case ImageChannel::G:
            hist_ior = qRgba(0, 255, 0, 255);
            bits = g;
            break;
        case ImageChannel::B:
            hist_ior = qRgba(0, 0, 255, 255);
            bits = b;
            break;
    }

    const int gray_level = 256;
    int hist[gray_level] = {0};

    // calculate histogram
    for (int i=0; i<width*height; i++)
    {
       int val = bits[i];
       hist[val]++;
    }

    // compress histogram into hist_image height
    int max_hist_val = hist[0];
    for (int i=1; i<gray_level; i++)
    {
        max_hist_val = hist[i] > max_hist_val ? hist[i] : max_hist_val;
    }

    int s_w = 2;
    int w = s_w*gray_level;
    int h = w;//width*height;
    float s_h = 0.8;

    for (int i=0; i<gray_level; i++)
    {
        int v = hist[i];
        hist[i] = int(h*1.0/max_hist_val * s_h * v);
    }

    QImage hist_image(w, h, QImage::Format_RGBA8888);
    QRgb value;
    for (int j=0; j<h; j++)
    {
        for (int i=0; i<w; i++)
        {
            if ( (hist[i/s_w] > 0) && (j >= h-hist[i/s_w]) )
                value = hist_ior;
            else
                value = qRgba(255, 255, 255, 255);
            hist_image.setPixel(i, j, value);
        }
    }
    delete [] r;
    delete [] g;
    delete [] b;

    return hist_image;
}

QImage calculateNegative(QImage &image, ImageChannel channel)
{
    // obtain gray image
    int width = image.width();
    int height = image.height();

    QImage newImage = image;
    int r, g, b;
    for (int j=0; j<height; j++) {
        for (int i=0; i<width; i++) {
            QRgb pixel = image.pixel(i, j);
            r = qRed(pixel);
            g = qGreen(pixel);
            b = qBlue(pixel);
            QRgb newPixel;
            //根据输入条件进行负片处理
            switch (channel) {
                case ImageChannel::Y:
                    newPixel = qRgb(255-r, 255-g, 255-b);
                    break;
                case ImageChannel::R:
                    newPixel = qRgb(255-r, g, b);
                    break;
                case ImageChannel::G:
                    newPixel = qRgb(r, 255-g, b);
                    break;
                case ImageChannel::B:
                    newPixel = qRgb(r, g, 255-b);
                    break;
            }
            newImage.setPixel(i, j, newPixel);
          }
    }

    return newImage;
}

QImage convertToPseudoColor(QImage &image, ColorMap map)
{
    int width = image.width();
    int height = image.height();
    // obtain gray image
    QImage grayImage = image.convertToFormat(QImage::Format_Grayscale8);

    // seperate RGB channel
    QImage newImage(width, height, QImage::Format_RGB888);
    for (int j=0; j<height; j++) {
        for (int i=0; i<width; i++) {
            QRgb pixel = grayImage.pixel(i, j);
            QRgb newPixel;
            int index, r, g, b;
            index = qRed(pixel); // gray scale
            r = qRed(pixel);
            g = qGreen(pixel);
            b = qBlue(pixel);
            switch (map) {// 根据映射方案由灰度生成RGB值
                case ColorMap::Jet:
                    r = jet_table[index*3];
                    g = jet_table[index*3+1];
                    b = jet_table[index*3+2];
                    break;
                case ColorMap::Parula:
                    r = parula_table[index*3];
                    g = parula_table[index*3+1];
                    b = parula_table[index*3+2];
                    break;
                case ColorMap::Hot:
                    r = hot_table[index*3];
                    g = hot_table[index*3+1];
                    b = hot_table[index*3+2];
                    break;
            }
            newPixel = qRgb(r, g, b);
            newImage.setPixel(i, j, newPixel);
          }
    }
    return newImage;
}
// 首先将RGB转至YCrCb空间，单独对Y通道进行灰度均衡，尽量避免色调改变，减少运算量
//算法有问题？均衡后色彩异常

QImage equalizeHistogramProc1(QImage &image)
{
    int width = image.width();
    int height = image.height();
    int pixel_num = width*height;

    // obtain image channels
    uchar *channels = new uchar[pixel_num*3];
    uchar *r = channels;
    uchar *g = channels+pixel_num;
    uchar *b = channels+2*pixel_num;
    splitImageChannel(image, r, g, b);

    // rgb to ycrcb
    float *ycrcb = new float[pixel_num*3];
    float *y = ycrcb;
    float *cr = ycrcb+pixel_num;
    float *cb = ycrcb+2*pixel_num;
    rgb2ycrcb(r, g, b, pixel_num, y, cr, cb);

    // calculate hist/pdf
    int *hist = new int[pixel_num]; // hist/pdf
    const int gray_level = 256;
    float *gray_distribution = new float[gray_level];// cdf

    uchar *gray_equal = new uchar[gray_level]; // equalized gray

        // calculate pdf
        memset(hist, 0, pixel_num*sizeof(int));
        for (int i=0; i<pixel_num; i++)
        {
            int index = (int)y[i]; // gray scale
            hist[index]++;
        }

        // calculate cdf
        memset(gray_distribution, 0, gray_level*sizeof(float));
        gray_distribution[0] = hist[0]*1.0f/pixel_num;
        for (int i = 1; i < gray_level; i++)
        {
            gray_distribution[i] = gray_distribution[i-1] + hist[i]*1.0f/pixel_num;
        }

        // recalculate equalized gray
        memset(gray_equal, 0, gray_level*sizeof(uchar));
        for (int i = 0; i < gray_level; i++)
        {
            gray_equal[i] = (uchar)(255 * gray_distribution[i] + 0.5);
        }

        // new gray channel
        for (int i=0; i<pixel_num; i++)
        {
            int index = (int)y[i]; // gray scale
            y[i] = gray_equal[index];
        }

        // ycrcb to rgb
    ycrcb2rgb(y, cr, cb, pixel_num, r, g, b);

    // update image
    int count = 0;
    QImage newImage = image;//grayImage;
    for (int j=0; j<height; j++) {
        for (int i=0; i<width; i++) {
            int nr = r[count];
            int ng = g[count];
            int nb = b[count];
            count++;
            newImage.setPixel(i, j, qRgb(nr,ng,nb));
        }
    }
    delete [] gray_equal;
    delete [] gray_distribution;
    delete [] channels;
    delete [] ycrcb;

    return newImage;
}
//对RGB三通道全部均衡，色调改变明显，效果不好

QImage equalizeHistogramProc(QImage &image)
{
    int width = image.width();
    int height = image.height();
    int pixel_num = width*height;

    // obtain gray image
    uchar *channels = new uchar[width*height*3];
    uchar *r = channels;
    uchar *g = channels+width*height;
    uchar *b = channels+2*width*height;
    splitImageChannel(image, r, g, b);

    uchar *c[4] = {r, g, b, 0};

    // calculate hist/pdf
    int *hist = new int[pixel_num]; // hist/pdf
    const int gray_level = 256;
    float *gray_distribution = new float[gray_level];// cdf

    uchar *gray_equal = new uchar[gray_level]; // equalized gray
    for (uchar **p=c; (*p) != 0; p++)
    {
        // calculate pdf
        memset(hist, 0, pixel_num*sizeof(int));
        for (int i=0; i<pixel_num; i++)
        {
            int index = (*p)[i]; // gray scale
            hist[index]++;
        }

        // calculate cdf
        memset(gray_distribution, 0, gray_level*sizeof(float));
        gray_distribution[0] = hist[0]*1.0f/pixel_num;
        for (int i = 1; i < gray_level; i++)
        {
            gray_distribution[i] = gray_distribution[i-1] + hist[i]*1.0f/pixel_num;
        }

        // recalculate equalized gray
        memset(gray_equal, 0, gray_level*sizeof(uchar));
        for (int i = 0; i < gray_level; i++)
        {
            gray_equal[i] = (uchar)(255 * gray_distribution[i] + 0.5);
        }

        // new gray channel
        for (int i=0; i<pixel_num; i++)
        {
            int index = (*p)[i]; // gray scale
            (*p)[i] = gray_equal[index];
        }
    }

    // update image
    int count = 0;
    QImage newImage = image;//grayImage;
    for (int j=0; j<height; j++) {
        for (int i=0; i<width; i++) {
            int nr = r[count];
            int ng = g[count];
            int nb = b[count];
            count++;
            newImage.setPixel(i, j, qRgb(nr,ng,nb));
        }
    }
    delete [] gray_equal;
    delete [] gray_distribution;
    delete [] channels;

    return newImage;
}



void filter(uchar *src, int image_width, int image_height,
            float *kernel, int kernel_width, int kernel_height, uchar *dst)
{
    int i, j, m, n;
    float val;

    memset(dst, 0, image_width*image_height);

    for (j=kernel_height/2; j<image_height-kernel_height/2; j++)
    {
        for (i = kernel_width/2; i<image_width-kernel_width/2; i++)
        {
            val = 0;
            for (n=-kernel_height/2; n<=kernel_height/2; n++)
            {
                for (m=-kernel_width/2; m<=kernel_width/2; m++)
                {
                    val += src[(j-n)*image_width+(i-m)] *
                            kernel[(n+kernel_height/2)*kernel_width+m+kernel_width/2];
                }
            }
            val = val>255 ? 255 : val;
            dst[j*image_width+i] = (uchar)(val<0 ? 0 : val);
        }
    }
}

//首先计算局部区域的均值与标准差(类似均值滤波，先得到基准)
//根据增益系数与原像素与基准差值对局部进行增强
//缺点，参数选择不当时噪声被极度放大(滤波器尺寸过小时)

void adaptiveContrastEnhancement(QImage &src_image, float *rgb, float *rgb_ii, float *rgb_ii_power, int max_window_size,
                                 int half_window_size, float alpha, float max_cg, QImage &dst_image)
{
    dst_image = src_image;
    int image_width = src_image.width();
    int image_height = src_image.height();
    int pixel_num = image_width*image_height;

    int max_image_width = src_image.width() + 2*max_window_size;
    int max_image_height = src_image.height() + 2*max_window_size;
    int max_pixel_num = max_image_width*max_image_height;
    int max_kernel_height = 2*max_window_size+1;
    int max_kernel_width = 2*max_window_size+1;

    int i=0, j=0;
    int kernel_height = 2*half_window_size+1;
    int kernel_width = 2*half_window_size+1;
    int kernel_size = kernel_height*kernel_width;
    float image_mean=0, image_std=0;


    for (int c=0; c<3; c++)
    {
        // image mean
        image_mean = box_integral(rgb_ii+c*max_pixel_num, max_image_width, max_image_height,
                               max_window_size, max_window_size + image_width-1,
                               max_window_size, max_window_size + image_height-1);
        image_mean /= pixel_num;

        // image std
        image_std = box_integral(rgb_ii_power+c*max_pixel_num, max_image_width, max_image_height,
                               max_window_size, max_window_size + image_width-1,
                               max_window_size, max_window_size + image_height-1);
        image_std /= pixel_num;
        image_std -= image_mean*image_mean;

        image_std = sqrtf(image_std);

        // local area mean and std
        for (j=max_kernel_height/2; j<max_image_height-max_kernel_height/2; j++)
        {
            for (i=max_kernel_width/2; i<max_image_width-max_kernel_width/2; i++)
            {
                // mean
                float mean = box_integral(rgb_ii+c*max_pixel_num, max_image_width, max_image_height,
                                       i-kernel_width/2, i+kernel_width/2,
                                       j-kernel_height/2, j+kernel_height/2);
                mean /= kernel_size;

                // std
                float std= box_integral(rgb_ii_power+c*max_pixel_num, max_image_width, max_image_height,
                                       i-kernel_width/2, i+kernel_width/2,
                                       j-kernel_height/2, j+kernel_height/2);
                std = std/kernel_size - mean*mean;
                std = sqrtf(std);

                // constrast gain
                float cg = alpha*image_std/std;
                if (cg>max_cg) cg = max_cg;

                float dst_val = mean + cg * (rgb[c*max_pixel_num + j*max_image_width+i] - mean);
                if (dst_val > 255) dst_val = 255;
                if (dst_val < 0) dst_val = 0;
                QRgb temp = dst_image.pixel(i-max_kernel_width/2, j-max_kernel_height/2);
                int temp_r = qRed(temp);
                int temp_g = qGreen(temp);
                int temp_b = qBlue(temp);
                if (c==0) temp_r = dst_val;
                if (c==1) temp_g = dst_val;
                if (c==2) temp_b = dst_val;
                dst_image.setPixel(i-max_kernel_width/2, j-max_kernel_height/2, qRgb(temp_r, temp_g, temp_b));
            }
        }
    }
}

//为了降低时间复杂度和操作次数，首先计算积分图像

void calculate_integral_image(float *image, int width, int height, float *integral_image)
{
    int i, j;

    // first row only
    float rs = 0;
    for(j=0; j<width; j++)
    {
        rs += image[j];
        integral_image[j] = rs;
    }
    for(i=1; i<height; ++i)
    {
        rs = 0;
        for(j=0; j<width; ++j)
        {
            rs += image[i*width+j];
            integral_image[i*width+j] = rs + integral_image[(i-1)*width+j];
        }
    }
}

void calculate_integral_image_power(float *image, int width, int height, float *integral_image)
{
    int i, j;

    // first row only
    float rs = 0;
    for(j=0; j<width; j++)
    {
        rs += image[j]*image[j];
        integral_image[j] = rs;
    }
    for(i=1; i<height; ++i)
    {
        rs = 0;
        for(j=0; j<width; ++j)
        {
            rs += image[i*width+j]*image[i*width+j];
            integral_image[i*width+j] = rs + integral_image[(i-1)*width+j];
        }
    }
}
__inline float box_integral(float *integral_image, int width, int height, int c1, int c2, int r1, int r2)
{
    float a, b, c, d;

    a = (c1-1<0 || r1-1<0)	? 0 : integral_image[(r1-1) * width + (c1-1)];
    b = r1-1<0				? 0 : integral_image[(r1-1) * width + c2];
    c = c1-1<0				? 0 : integral_image[r2 * width + (c1-1)];
    d = integral_image[r2 * width + c2];

    return a - b - c + d;
}
//基于Roberts算子的边缘检测,scale为对差分结果进行缩放
QImage edgeDetectRoberts(QImage &image, double scale)
{
    QImage newImage(image.width(),image.height(),QImage::Format_ARGB32);
    QColor color0;
    QColor color1;
    QColor color2;
    QColor color3;
    int  r = 0;
    int g = 0;
    int b = 0;
    int rgb = 0;
    int r1 = 0;
    int g1 = 0;
    int b1 = 0;
    int rgb1 = 0;
    int a = 0;
    for( int y = 0; y < image.height() - 1; y++)
    {
        for(int x = 0; x < image.width() - 1; x++)
        {
            color0 =   QColor ( image.pixel(x,y));
            color1 =   QColor ( image.pixel(x + 1,y));
            color2 =   QColor ( image.pixel(x,y + 1));
            color3 =   QColor ( image.pixel(x + 1,y + 1));
            r = abs(color0.red() - color3.red());
            g = abs(color0.green() - color3.green());
            b = abs(color0.blue() - color3.blue());
            rgb = r + g + b;

            r1 = abs(color1.red() - color2.red());
            g1= abs(color1.green() - color2.green());
            b1 = abs(color1.blue() - color2.blue());
            rgb1 = r1 + g1 + b1;

            a = rgb + rgb1;
            a = a * scale;
            a = a>255?255:a;

            newImage.setPixel(x,y,qRgb(a,a,a));
        }
    }
    return newImage;
}

/*基于sobel算子边缘检测 比例scale对差分结果进行缩放,type表示使用那种公式 取值0或则1*/
QImage edgeDetectSobel(QImage &image, double scale, int type)
{
    QImage newImage(image.width(),image.height(),QImage::Format_ARGB32);
    QColor color0;
    QColor color1;
    QColor color2;
    QColor color3;
    QColor color4;
    QColor color5;
    QColor color6;
    QColor color7;
    QColor color8;
    int  r = 0;
    int g = 0;
    int b = 0;
    int rgb = 0;
    int r1 = 0;
    int g1 = 0;
    int b1 = 0;
    int rgb1 = 0;
    int a = 0;
    for( int y = 1; y < image.height() - 1; y++)
    {
        for(int x = 1; x < image.width() - 1; x++)
        {
            color0 =   QColor ( image.pixel(x,y));

            color1=   QColor ( image.pixel(x-1,y-1));
            color2 =   QColor ( image.pixel(x,y-1));
            color3 =   QColor ( image.pixel(x+1,y));

            color4 =   QColor ( image.pixel(x-1,y));
            color5 =   QColor ( image.pixel(x+1,y));

            color6 =   QColor ( image.pixel(x-1,y+1));
            color7=   QColor ( image.pixel(x,y+1));
            color8 =   QColor ( image.pixel(x+1,y+1));
            r = abs(color1.red() + color2.red() * 2 + color3.red() - color6.red() - color7.red() * 2 - color8.red());
            g = abs(color1.green() + color2.green() * 2 + color3.green() - color6.green() - color7.green() * 2 - color8.green());
            b = abs(color1.blue() + color2.blue() * 2 + color3.blue() - color6.blue() - color7.blue() * 2 - color8.blue());
            rgb = r + g + b;

            r1 = abs(color1.red() + color4.red() * 2 + color6.red() - color3.red() - color5.red() * 2 - color8.red());
            g1= abs(color1.green() + color4.green() * 2 + color6.green() - color3.green() - color5.green() * 2 - color8.green());
            b1 = abs(color1.blue() + color4.blue() * 2 + color6.blue() - color3.blue() - color5.blue() * 2 - color8.blue());
            rgb1 = r1 + g1 + b1;
           if(type == 0)
           {
               if (rgb > rgb1)
                   a = rgb;
               else
                   a = rgb1;
           }
           else if(type == 1)
           {
            a = (rgb + rgb1)/2;
           }
            a = a * scale;
            a = a>255?255:a;

            newImage.setPixel(x,y,qRgb(a,a,a));
        }
    }
    return newImage;
}
