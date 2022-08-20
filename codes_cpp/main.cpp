/**
 * @file yuvToPng.cpp
 * @author hyunmin cho (cho.hyun@icloud.com)
 * @brief make .YUV format sequence into .png image file, based on gnu++20
 * @version 0.11
 * @date 2022-07-19
 *
 * @copyright Copyright (c) 2022
 *
 */


#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <bitset>

#define TITLE first
#define FRAME second.second.first
#define WID second.second.second.first
#define HEI second.second.second.second


using namespace cv;
using namespace dnn;
namespace fs = std::filesystem;

typedef std::vector<std::string> str_vec;
typedef std::vector<Mat> matrix_vec;
typedef std::vector<std::pair<std::string, std::pair<uintmax_t, std::pair<double, std::pair<int, int>>>>> meta_vec;
typedef std::bitset<10> bit10;

str_vec
get_titles_of_each_yuv(fs::path& path);

meta_vec
get_metadata_of_each_yuv(fs::path& path, str_vec& titles_of_yuv, int wid, int hei);

bool
create_directory(fs::path& path, str_vec& titles_of_yuv);

void
yuv2png_8bit(meta_vec& mv, fs::path& yuv_root, fs::path& png_root);

void
yuv2png_10bit(meta_vec& mv, fs::path& yuv_root, fs::path& png_root);


int
main(int argc, char *argv[]) {

    /*
    Target YUV folder is in the same level of the file
    In YUVs folder, there SHOULD BE NOT EXIST the file that is NOT YUV,
    except METADATA Folder.

    |-- Target YUV folder (In default, YUVs)
        |--Sequence1
            |-- Sequence1.yuv
            |-- Sequence1.md5
            ...
        |--Sequence2
        |--Sequence3
        ...
        |--METADATA
    |-- yuvToPng.cpp
    */


    // Set folder path where your .YUV files exist.
    // Default setting: ../YUVs
    fs::path YUV_path(argv[1]);
    str_vec titles_of_yuv = get_titles_of_each_yuv(YUV_path);


    // Get metadata of each YUV, including titles.
    meta_vec metadata_of_yuv = get_metadata_of_each_yuv(YUV_path, titles_of_yuv,
                                                        (int) strtol(argv[3], nullptr, 10), (int) strtol(argv[4], nullptr, 10));


    // Directory where converted pngs will be stored into.
    // Feel free to change the directory where you want to place.
    fs::path PNG_path = argv[2];
    create_directory(PNG_path, titles_of_yuv);

    // Run by following each YUV bits.
    (int) strtol(argv[5], nullptr, 10) == 8 ?
    yuv2png_8bit(metadata_of_yuv, YUV_path, PNG_path):
    yuv2png_10bit(metadata_of_yuv, YUV_path, PNG_path);

    return EXIT_SUCCESS;
}

str_vec
get_titles_of_each_yuv(fs::path& path) {

    str_vec yuv_name;

    // do iteration of folder path,
    // and get each yuv file name and process yuv file into pngs.
    fs::directory_iterator itr(path);

    while (itr != fs::end(itr)) {
        const fs::directory_entry& entry = *itr;
        const std::string str_path = entry.path().generic_string();


        // Tokenize string into token
        std::istringstream iss(str_path);
        std::string token;
        int token_itr = 0;
        while (std::getline(iss, token, '/')) {
            token_itr++;
            if (token_itr == 3) {
                if (operator!=(token, "METADATA")) {
                    yuv_name.emplace_back(token);
                }
            }
        }
        itr++;
    }

    return yuv_name;
}

meta_vec
get_metadata_of_each_yuv(fs::path& path, str_vec& titles_of_yuv, int wid, int hei) {
    meta_vec metadata_vec;

    // get each YUVs size, to get each YUVs **frame size**
    for (auto& title : titles_of_yuv) {
        // initialize every iterate.
        fs::path base_path = path;

        base_path /= title;
        fs::path yuv = base_path /= title + ".yuv";

        uintmax_t file_size = fs::file_size(yuv);

        // store each YUVs TITLE, SIZES and FRAME numbers.
        metadata_vec.emplace_back(
                std::make_pair(title, std::make_pair
                (file_size, std::make_pair
                (file_size / (wid * hei * 3) / 2, std::make_pair
                (wid, hei)))));

    }

    return metadata_vec;
}

bool
create_directory(fs::path& path, str_vec& titles_of_yuv) {

    if (!fs::exists(path)) {
        fs::create_directory(path);
    }

    // make each YUVs folder.
    for (auto& title: titles_of_yuv) {
        fs::path base_inter_p = path;
        base_inter_p /= title; // gnu++17 operator... make it into as "base_inter_p/title"

        if (!fs::exists(base_inter_p)) {
            fs::create_directory(base_inter_p);
        }
    }

    return true;
}

void
yuv2png_8bit(meta_vec& mv, fs::path& yuv_root, fs::path& png_root) {
    // make each yuv into pngs (name_y00x.png, name_u00x.png, name_v00x.png)
    fs::path yuv_path(yuv_root);
    fs::path png_path(png_root);
    str_vec yuv_comp{"y", "u", "v"};

    for (auto& meta: mv) {

        yuv_path /= meta.TITLE;
        yuv_path /= meta.TITLE + ".yuv";
        png_path /= meta.TITLE;

        // declare wid and height of each yuv
        const int wid = meta.WID;
        const int hei = meta.HEI;

        const int y_f_size = wid * hei;
        const int uv_f_size = (wid / 2) * (hei / 2);

        FILE *fid = fopen(yuv_path.c_str(), "rb");

        // Total Y, U and V frame array
        auto Y = (uchar *) malloc(y_f_size);
        auto U = (uchar *) malloc(uv_f_size);
        auto V = (uchar *) malloc(uv_f_size);

        memset(Y, 0, sizeof(y_f_size));
        memset(U, 0, sizeof(uv_f_size));
        memset(U, 0, sizeof(uv_f_size));


        for (int i = 0; i < meta.FRAME; ++i) {
            fread(Y, sizeof(uchar), y_f_size, fid);
            fread(U, sizeof(uchar), uv_f_size, fid);
            fread(V, sizeof(uchar), uv_f_size, fid);


            matrix_vec yuv{Mat(hei, wid, CV_8UC1, Y), Mat(hei / 2, wid / 2, CV_8UC1, U), Mat(hei / 2, wid / 2, CV_8UC1, V)};

            // make it into .png file
            fs::path comp_path = png_path;
            for (int j = 0; j < 3; ++j) {
                comp_path /= (meta.TITLE + "_" + yuv_comp[j] + std::to_string(i) + ".png");
                imwrite(comp_path.generic_string(), yuv[j]);
                comp_path = png_path;
            }
        }

        // initialize root path for each epoch
        yuv_path = yuv_root;
        png_path = png_root;
    }
}

void
yuv2png_10bit(meta_vec& mv, fs::path& yuv_root, fs::path& png_root) {
    std::cout << "If you want to handle yuv4:2:0 10bit, then you should \n";
    std::cout << "run using python build. \n"; 
}
