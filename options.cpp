#include "options.h"

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
namespace po = boost::program_options;

void print_help(po::options_description &description,
                po::positional_options_description &pd) {
    std::string usage;
    for(unsigned int i = 0; i < pd.max_total_count(); i++) {
        usage += " ";
        usage += pd.name_for_position(i);
    }
    std::cout << "Usage: band_pass" + usage << std::endl;
    std::cout << description;
    exit(-1);
}

void handle_options(int argc, char **argv,
                    int &frame_offset,
                    int &reduce,
                    long long &max_frames,
                    std::string &input_file,
                    cv::VideoCapture &cap) {
    po::options_description description;
    std::string output_file;
    description.add_options()
        ("help", "Display this help message")
        ("reduce", po::value<int>(&reduce)->default_value(1),
            "Reduction factor")
        ("frame-offset", po::value<int>(&frame_offset)->default_value(0), "Frame initial offset")
        ("max-frames", po::value<long long>(&max_frames),
            "Maximum number of frames to process")
        ("output-file", po::value<std::string>(&output_file)->default_value(""),
            "Output file")
        ("webcam", "Use Webcam")
        ("input-file", "Location of movie");

    po::positional_options_description pd;
    pd.add("input-file", 1);

    // Parse
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
                options(description).positional(pd).run(), vm);
    po::notify(vm);
    if(vm.count("help")) {
        print_help(description, pd);
    }

    if(!vm.count("input-file") && !vm.count("webcam")) {
        print_help(description, pd);
    }

    if (!vm.count("webcam") ) {
        input_file = vm["input-file"].as<std::string>();
    }

    if (vm.count("webcam")) {
        cap.open(0);
    } else {
        cap.open(input_file);
    }
    if (!cap.isOpened()) {
        std::cout << "Failed to open " << input_file << std::endl;
        if (!boost::filesystem::exists(input_file)) {
            std::cout << "File does not exist." << std::endl;;
        } else {
            std::cout << "Filetype not supported, make sure that OpenCV has"
                         " ffmpeg support." << std::endl;
        }
        exit(-1);
    }
}
