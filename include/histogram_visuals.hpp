#include <histogram.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

template <class K>
class HistogramVisuals {
    public:
        static void show_internals(Histogram<K>& hist, std::string window_name) {
            int height = hist.bin_count() / 2;
            cv::Mat display = cv::Mat::zeros(height, hist.bin_count(), CV_8UC1);

            int max = 0;
            for (int x = 0; x < hist.bin_count(); x++)
                if (hist.bins[x] > max) max = hist.bins[x];
            if (max == 0) return;

            for (int x = 0; x < hist.bin_count(); x++) {
                int n_pix = height - ((hist.bins[x] * height) / max);
                if (n_pix < 0) n_pix = 0;
                for (int y = height - 1; y > n_pix; y--) {
                    display.at<unsigned char>(y, x) = 255;
                }
            }
            imshow(window_name, display);
        }
};
