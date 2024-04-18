// Authors : khaled Mohamed Ibrahim 20230130 - Mohamed Hasan Aboserra 20230329 - Mohamed Mostafa Badry 20230357
// Emails : khaledhawash143@gmail.com - mohamedhasa88888@gmail.com - mohammadbadry5@gmail.com
// Who did what :
//               khaled mohamed ibrahim : 1. Grayscale Conversion, 4. Merg, 7. brightness and darkness, 10. Detect Edges, 15. TV, 16. Purple, 19. Lavender Dream and 20. Mirror In The Middle
//               mohamed mostafa badry  : 3. Invert image, 6. Rotation, 9. Frame, 12. Blur, 13. Sunlight, 14. Oil Painting, 17. Inrared and 18. Image skew
//               mohamed hasan aboserra : 2. black and white, 5. Flip, 8. Crop and 11. resize
//               the menu and the GUI was made by all of us
// Section number : s5 / s6

#include "bits/stdc++.h"
#include "Image_Class-1.h"

using namespace std;
namespace fs = std::filesystem;

bool isnumbers(string userValue1, string userValue2) {
    for (char item : userValue1) {
        if (!isdigit(item)) {
            return false;
        }
    }
    for (char item : userValue2) {
        if (!isdigit(item)) {
            return false;
        }
    }
}

bool isValidExtension(const string& filename) {
    string extn = filename.substr(filename.size() - 4);
    return (extn == ".jpg" || extn == ".bmp" || extn == ".png" || extn == ".tga");
}

bool fileExists(const string& filename) {
    return fs::exists(filename);
}

void addNoise(Image& image, int intensity) {
    // Add noise to each pixel
    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height; j++) {
            int noise = rand() % (2 * intensity + 1) - intensity;
            image(i, j, 0) = min(255, max(0, image(i, j, 0) + noise));
            image(i, j, 1) = min(255, max(0, image(i, j, 1) + noise));
            image(i, j, 2) = min(255, max(0, image(i, j, 2) + noise));
        }
    }
}
bool isValidDegree(string userValue) {
    for (char item : userValue) {
        if (!isdigit(item)) {
            return false;
        }
    }
    if (stof(userValue) <= 80 && stof(userValue) > 0) {
        return true;
    }
    else {
        return false;
    }
}
void applyMirrorEffect(Image& image) {
    int width = image.width;
    int height = image.height;
    int halfWidth = width / 2;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < halfWidth; ++x) {
            // Get pixel from the left side
            unsigned char& r_left = image(x, y, 0);
            unsigned char& g_left = image(x, y, 1);
            unsigned char& b_left = image(x, y, 2);

            // Get corresponding pixel from the right side
            unsigned char& r_right = image(width - x - 1, y, 0);
            unsigned char& g_right = image(width - x - 1, y, 1);
            unsigned char& b_right = image(width - x - 1, y, 2);

            // Set the pixel on the right side to the pixel on the left side
            r_right = r_left;
            g_right = g_left;
            b_right = b_left;
        }
    }
}
void flipVertical(Image& img) {
    for (int y = 0; y < img.height / 2; y++) {
        for (int x = 0; x < img.width; x++) {
            for (int c = 0; c < img.channels; c++) {
                swap(img.getPixel(x, y, c), img.getPixel(x, img.height - y - 1, c));
            }
        }
    }
}
// Function to resize an image to new dimensions
Image resizeImage(Image& image, int newWidth, int newHeight) {
    Image newImage(newWidth, newHeight);
    double scaleX = static_cast<double>(image.width) / newWidth;
    double scaleY = static_cast<double>(image.height) / newHeight;

    for (int i = 0; i < newWidth; ++i) {
        for (int j = 0; j < newHeight; ++j) {
            int origX = round(i * scaleX);
            int origY = round(j * scaleY);
            for (int k = 0; k < 3; k++) {
                newImage(i, j, k) = image(origX, origY, k);
            }
        }
    }
    return newImage;
}


void flipHorizontal(Image& img) {
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width / 2; x++) {
            for (int c = 0; c < img.channels; c++) {
                swap(img.getPixel(x, y, c), img.getPixel(img.width - x - 1, y, c));
            }
        }
    }
}

void applyPurpleFilter(Image& img) {
    for (int y = 0; y < img.height; ++y) {
        for (int x = 0; x < img.width; ++x) {

            unsigned char& r = img.getPixel(x, y, 0);
            unsigned char& g = img.getPixel(x, y, 1);
            unsigned char& b = img.getPixel(x, y, 2);
            r = min(255, static_cast<int>(r * 1.25));
            g = min(255, static_cast<int>(g * 0.75));
            b = min(255, static_cast<int>(b * 1.25));
        }
    }
}

bool isValidInput(string number) {
    for (char item : number) {
        if (!isdigit(item))
            return false;
    }
    if (stoi(number) < 0 || stoi(number) % 2 == 0 || stoi(number) == 1)
        return false;
    else
        return true;
}

// Sobel operator kernels
int sobel_x[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
};

int sobel_y[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
};

void applySobel(Image& inputImage, Image& outputImage) {
    for (int x = 1; x < inputImage.width - 1; ++x) {
        for (int y = 1; y < inputImage.height - 1; ++y) {
            int gradient_x = 0;
            int gradient_y = 0;
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    gradient_x += inputImage.getPixel(x + i, y + j, 0) * sobel_x[i + 1][j + 1];
                    gradient_y += inputImage.getPixel(x + i, y + j, 0) * sobel_y[i + 1][j + 1];
                }
            }
            int gradient = sqrt(gradient_x * gradient_x + gradient_y * gradient_y);
            gradient = min(255, max(0, gradient)); // Apply thresholding
            // Invert gradient to make edges black
            gradient = 255 - gradient;
            for (int k = 0; k < 3; k++) {
                outputImage.setPixel(x, y, k, gradient);
            }
        }
    }
}


string getUserOption() {
    string option;
    while (true) {
        cout << "Which Filter Do you want to do today?\n";
        cout << "1. Grayscale Convertion\n";
        cout << "2. Convert to Black and White\n";
        cout << "3. Invert Image" << endl;
        cout << "4. Merge Two Images" << endl;
        cout << "5. Flip Image" << endl;
        cout << "6. Rotation" << endl;
        cout << "7. Darken And Lighten" << endl;
        cout << "8. Croping\n";
        cout << "9. Image Frame\n";
        cout << "10. Detect Edges\n";
        cout << "11. Resizing" << endl;
        cout << "12. Blur" << endl;
        cout << "13. Sunlight\n";
        cout << "14. Oil Painting\n";
        cout << "15. TV Effect" << endl;
        cout << "16. Do You want Your Imgae Look Purple" << endl;
        cout << "17. Infrared\n";
        cout << "18. Image Skew\n";
        cout << "19. Lavender Dream" << endl;
        cout << "20. Mirror in the middle\n";
        cout << "21. Exit" << endl;
        cout << "Please enter the option number: ";

        cin >> option;

        if (cin.fail() || (option != "1" && option != "2" && option != "3" && option != "4" && option != "5" && option != "6" && option != "7" && option != "8" && option != "9" && option != "10" && option != "11"s && option != "12" && option != "13" && option != "14" && option != "15" && option != "16" && option != "17" && option != "18" && option != "19" && option != "20" && option != "21")) {
            cout << "Invalid input. Please enter a number between 1 and 20." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else
        {
            break;
        }
    }
    return option;
}

int main()
{
    string filename;
    string option;
    while (true) {
        option = getUserOption();
        // Display the menu
        if (option == "1")
        {
            string outputFilename;
            while (true) {
                cout << "Please enter the image name with its extension: ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                // Check if the file exists
                if (!fileExists(filename)) {
                    cout << "The file '" << filename << "' does not exist. Please try again." << endl;
                    continue;
                }

                break;
            }

            Image image(filename);
            // Loop through each pixel of the image
            for (int y = 0; y < image.height; ++y)
            {
                for (int x = 0; x < image.width; ++x)
                {
                    // Calculate the grayscale value for the current pixel
                    unsigned int grayValue = 0;
                    for (int c = 0; c < image.channels; ++c)
                    {
                        grayValue += image(x, y, c); // Sum up the pixel values for each channel
                    }
                    grayValue /= image.channels; // Calculate the average pixel value for grayscale

                    // Set all channels of the pixel to the grayscale value
                    for (int c = 0; c < image.channels; ++c)
                    {
                        image(x, y, c) = grayValue; // Assign the grayscale value to each channel
                    }
                }
            }
            while (true) {
                cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                cin >> outputFilename;


                if (outputFilename.size() < 4 || !isValidExtension(outputFilename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                break;
            }
            image.saveImage(outputFilename); // Save the grayscale image with the specified filename
            cout << "Grayscale conversion completed successfully and the pic is saved as : " << outputFilename << endl;
        }

        else if (option == "2")
        {
            while (true) {
                cout << "Please enter the image name with its extension: ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                // Check if the file exists
                if (!fileExists(filename)) {
                    cout << "The file '" << filename << "' does not exist. Please try again." << endl;
                    continue;
                }

                break;
            }

            Image image(filename);
            for (int i = 0; i < image.width; ++i)
            {
                for (int j = 0; j < image.height; ++j)
                {
                    unsigned int avg = 0; // Initialize average value

                    for (int k = 0; k < 3; ++k)
                    {
                        avg += image(i, j, k); // Accumulate pixel values
                    }

                    avg /= 3; // Calculate average

                    if (avg < 128)
                    {
                        // Set pixel to black
                        for (int k = 0; k < 3; ++k)
                        {
                            image(i, j, k) = 0;
                        }
                    }
                    else
                    {
                        // Set pixel to white
                        for (int k = 0; k < 3; ++k)
                        {
                            image(i, j, k) = 255;
                        }
                    }
                }
            }

            string outputFilename;
            while (true) {
                cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                cin >> outputFilename;


                if (outputFilename.size() < 4 || !isValidExtension(outputFilename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                break;
            }

            image.saveImage(outputFilename);
            cout << "Your picture has been inverted successfully and saved as: " << outputFilename << endl;

        }

        else if (option == "3")
        {
            while (true) {
                cout << "Please enter the image name with its extension: ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                // Check if the file exists
                if (!fileExists(filename)) {
                    cout << "The file '" << filename << "' does not exist. Please try again." << endl;
                    continue;
                }

                break;
            }

            Image image(filename);
            for (int i = 0; i < image.width; ++i) {
                for (int j = 0; j < image.height; ++j) {
                    for (int k = 0; k < 3; ++k) {
                        image(i, j, k) = -(image(i, j, k) - 255);
                    }
                }
            }

            string outputFilename;
            while (true) {
                cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                cin >> outputFilename;


                if (outputFilename.size() < 4 || !isValidExtension(outputFilename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                break;
            }

            image.saveImage(outputFilename);
            cout << "Your picture has been inverted successfully and saved as: " << outputFilename << endl;

        }

        else if (option == "4") {
            string choice;
            string filename1, filename2;
            while (true) {
                cout << "Please enter the image name with its extension: ";
                cin >> filename1;


                if (filename1.size() < 4 || !isValidExtension(filename1)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                // Check if the file exists
                if (!fileExists(filename1)) {
                    cout << "The file '" << filename1 << "' does not exist. Please try again." << endl;
                    continue;
                }

                break;
            }
            Image img1(filename1);
            while (true) {
                cout << "Please enter the image name with its extension: ";
                cin >> filename2;


                if (filename2.size() < 4 || !isValidExtension(filename2)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                // Check if the file exists
                if (!fileExists(filename2)) {
                    cout << "The file '" << filename2 << "' does not exist. Please try again." << endl;
                    continue;
                }

                break;
            }
            Image img2(filename2);
            cout << "do you want to crop the merged image or resize it " << endl;
            cout << "1. crop" << endl;
            cout << "2. resize" << endl;
            cin >> choice;
            if (choice == "1")
            {
                int mergedWidth = min(img1.width, img2.width);
                int mergedHeight = min(img1.height, img2.height);

                Image image(mergedWidth, mergedHeight);


                for (int y = 0; y < mergedHeight; ++y) {
                    for (int x = 0; x < mergedWidth; ++x) {
                        for (int c = 0; c < image.channels; ++c) {
                            unsigned char pixelValue = (img1(x, y, c) + img2(x, y, c)) / 2;
                            image(x, y, c) = pixelValue;
                        }
                    }
                }



                while (true) {
                    cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                    cin >> filename;


                    if (filename.size() < 4 || !isValidExtension(filename)) {
                        cout << "Invalid filename or extension. Please try again." << endl;
                        continue;
                    }

                    break;
                }
                image.saveImage(filename);

                cout << "Your pic has been filtered successfully and it is saved as : " << filename << endl;

            }
            else if (choice == "2")
            {
                int newWidth = max(img1.width, img2.width);
                int newHeight = max(img1.height, img2.height);

                // Resize images
                img1 = resizeImage(img1, newWidth, newHeight);
                img2 = resizeImage(img2, newWidth, newHeight);

                // Merge images
                Image image(newWidth, newHeight);
                for (int y = 0; y < newHeight; ++y) {
                    for (int x = 0; x < newWidth; ++x) {
                        for (int c = 0; c < image.channels; ++c) {
                            // Ensure pixel values are averaged correctly
                            unsigned char pixelValue = (img1(x, y, c) + img2(x, y, c)) / 2;
                            image(x, y, c) = pixelValue;
                        }
                    }
                }

                while (true) {
                    cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                    cin >> filename;


                    if (filename.size() < 4 || !isValidExtension(filename)) {
                        cout << "Invalid filename or extension. Please try again." << endl;
                        continue;
                    }

                    break;
                }
                image.saveImage(filename);

                cout << "Your pic has been filtered successfully and it is saved as : " << filename << endl;







            }

        }

        else if (option == "5")
        {

            string choice;
            while (true) {
                cout << "Please enter the image name with its extension: ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                // Check if the file exists
                if (!fileExists(filename)) {
                    cout << "The file '" << filename << "' does not exist. Please try again." << endl;
                    continue;
                }

                break;
            }
            Image image(filename);

            cout << "Do you want to flip the image vertically or horizontally? (1/2): ";
            cin >> choice;
            while (choice != "1" && choice != "2") {
                cout << "please choose a vaild number either 1 or 2 ";
                cin >> choice;
            }


            if (choice == "1") {
                flipVertical(image);
            }
            else if (choice == "2") {
                flipHorizontal(image);
            }

            while (true) {
                cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                break;
            }
            image.saveImage(filename);

            cout << "Your pic has been filtered successfully and it is saved as : " << filename << endl;

        }

        else if (option == "6")
        {

            while (true) {
                cout << "Please enter the image name with its extension: ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                // Check if the file exists
                if (!fileExists(filename)) {
                    cout << "The file '" << filename << "' does not exist. Please try again." << endl;
                    continue;
                }

                break;
            }

            Image image1(filename);
            cout << "** Rotation Filter **" << endl;
            cout << "1- Rotate by 90  degrees" << endl;
            cout << "2- Rotate by 180 degrees" << endl;
            cout << "3- Rotate by 270 degrees" << endl;

            string choice;
            cin >> choice;
            while (choice != "1" && choice != "2" && choice != "3") {
                cout << "Invalid, please enter a number between 1 and 3" << endl;
                cin >> choice;
            }

            if (choice == "1") {
                Image image2(image1.height, image1.width);
                for (int i = 0; i < image1.width; i++) {
                    for (int j = 0; j < image1.height; j++) {
                        for (int k = 0; k < 3; k++)
                            // rotation process
                            image2(image1.height - 1 - j, i, k) = image1(i, j, k);
                    }
                }
                while (true) {
                    cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                    cin >> filename;


                    if (filename.size() < 4 || !isValidExtension(filename)) {
                        cout << "Invalid filename or extension. Please try again." << endl;
                        continue;
                    }

                    break;
                }
                image2.saveImage(filename);
            }

            else if (choice == "2") {
                Image image2(image1.width, image1.height);
                for (int i = 0; i < image1.width; i++) {
                    for (int j = 0; j < image1.height; j++) {
                        for (int k = 0; k < 3; k++)
                            // rotation process
                            image2(image1.width - i - 1, image1.height - j - 1, k) = image1(i, j, k);
                    }
                }
                while (true) {
                    cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                    cin >> filename;


                    if (filename.size() < 4 || !isValidExtension(filename)) {
                        cout << "Invalid filename or extension. Please try again." << endl;
                        continue;
                    }

                    break;
                }
                image2.saveImage(filename);
            }

            else if (choice == "3") {
                Image image2(image1.height, image1.width);
                for (int i = 0; i < image1.width; i++) {
                    for (int j = 0; j < image1.height; j++) {
                        for (int k = 0; k < 3; k++)
                            // rotation process
                            image2(j, image1.width - i - 1, k) = image1(i, j, k);
                    }
                }
                while (true) {
                    cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                    cin >> filename;


                    if (filename.size() < 4 || !isValidExtension(filename)) {
                        cout << "Invalid filename or extension. Please try again." << endl;
                        continue;
                    }

                    break;
                }
                image2.saveImage(filename);
            }


        }

        else if (option == "7")
        {
            string outputFilename;
            string filterChoice;

            // Request user input for the filename
            while (true) {
                cout << "Please enter the image name with its extension: ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                // Check if the file exists
                if (!fileExists(filename)) {
                    cout << "The file '" << filename << "' does not exist. Please try again." << endl;
                    continue;
                }

                break;
            }

            Image image(filename);

            // Create a new Image instance for the filtered image
            Image filteredImage(image.width, image.height);

            // Request user input for the filter choice
            cout << "Please choose the filter to apply:" << endl;
            cout << "1. Brightness" << endl;
            cout << "2. Darkness" << endl;
            cin >> filterChoice;
            while (filterChoice != "1" && filterChoice != "2")
            {
                cout << "Invalid, please enter 1 or 2" << endl;
                cin >> filterChoice;
            }
            // Request user input for the output filename
            while (true) {
                cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                cin >> outputFilename;


                if (outputFilename.size() < 4 || !isValidExtension(outputFilename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                break;
            }

            // Loop through each pixel of the original image
            for (int y = 0; y < image.height; ++y)
            {
                for (int x = 0; x < image.width; ++x)
                {
                    for (int c = 0; c < image.channels; ++c)
                    {
                        // Apply the selected filter
                        if (filterChoice == "1")
                        {
                            // Apply brightness filter
                            unsigned int filteredValue =
                                    image(x, y, c) + 50; // Increase pixel value by 50 for brighter effect
                            filteredImage(x, y, c) = (filteredValue > 255) ? 255
                                                                           : filteredValue; // Limit pixel value to 255 (maximum brightness)
                        }
                        else
                        {
                            // Apply darkness filter
                            unsigned int filteredValue =
                                    image(x, y, c) * 0.5; // Reduce pixel value by 50% for darker effect
                            filteredImage(x, y, c) = filteredValue; // Assign the darker pixel value
                        }
                    }
                }
            }

            // Save the filtered image
            filteredImage.saveImage(outputFilename);

            // Print a success message with the output filename
            cout << "Filter applied successfully. Image saved: " << outputFilename << endl;
        }

        else if (option == "8")
        {
            while (true) {
                cout << "Please enter the image name with its extension: ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                // Check if the file exists
                if (!fileExists(filename)) {
                    cout << "The file '" << filename << "' does not exist. Please try again." << endl;
                    continue;
                }

                break;
            }
            Image image(filename);

            string userValue1 = "0", userValue2 = "0";
            int startX, startY, cropWidth, cropHeight;
            cout << "Enter the starting point (x, y) of the area to keep: ";
            cin >> userValue1 >> userValue2;

            while (!isnumbers(userValue1, userValue2) || stoi(userValue1) > image.width || stoi(userValue2) > image.height) {
                if (!isnumbers(userValue1, userValue2)) {
                    cout << "Please enter a valid starting point" << endl;
                    cin >> userValue1 >> userValue2;
                }
                else {
                    cout << "Size limit exceeded please enter starting points agine" << endl;
                    cin >> userValue1 >> userValue2;
                }
            }

            startX = stoi(userValue1);
            startY = stoi(userValue2);

            cout << "Enter the dimensions (width x height) of the area to crop: ";
            cin >> userValue1 >> userValue2;

            while (!isnumbers(userValue1, userValue2) || stoi(userValue1) > image.width || stoi(userValue1) == 0 || stoi(userValue2) > image.height || stoi(userValue2) == 0) {
                if (!isnumbers(userValue1, userValue2)) {
                    cout << "Please enter a valid starting point" << endl;
                    cin >> userValue1 >> userValue2;
                }
                else {
                    cout << "Size limit exceeded please enter starting points agine" << endl;
                    cin >> userValue1 >> userValue2;
                }
            }

            cropWidth = stoi(userValue1);
            cropHeight = stoi(userValue2);

            Image croppedImage(cropWidth, cropHeight);

            for (int i = 0; i < cropWidth; ++i)
            {
                for (int j = 0; j < cropHeight; ++j)
                {
                    for (int c = 0; c < 3; ++c)
                    {
                        croppedImage(i, j, c) = image(startX + i, startY + j, c);
                    }
                }
            }

            while (true) {
                cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                break;
            }
            croppedImage.saveImage(filename);

            cout << "Your pic has been filtered successfully and it is saved as : " << filename << endl;

        }

        else if (option == "9")
        {
            while (true) {
                cout << "Please enter the image name with its extension: ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                // Check if the file exists
                if (!fileExists(filename)) {
                    cout << "The file '" << filename << "' does not exist. Please try again." << endl;
                    continue;
                }

                break;
            }
            Image image(filename);

            cout << "What color do you want in the farme" << endl;
            cout << "1- black" << endl;
            cout << "2- Brown" << endl;
            cout << "3- white" << endl;
            cout << "4- Red" << endl;
            cout << "5- Green" << endl;
            cout << "6- Blue" << endl;
            cout << "7- yellow" << endl;
            cout << "8- Orange" << endl;
            cout << "9- purple" << endl;

            string color = "0";
            cin >> color;

            while (color != "1" && color != "2" && color != "3" && color != "4" && color != "5" && color != "6" && color != "7" && color != "8" && color != "9") {
                cout << "Please insert a valid color number." << endl;
                cout << "1- black" << endl;
                cout << "2- Brown" << endl;
                cout << "3- white" << endl;
                cout << "4- Red" << endl;
                cout << "5- Green" << endl;
                cout << "6- Blue" << endl;
                cout << "7- yellow" << endl;
                cout << "8- Orange" << endl;
                cout << "9- purple" << endl;
                cin >> color;
            }

            cout << "What frame do you want to add to your photo" << endl;
            cout << "1- simple frame" << endl;
            cout << "2- fancy frame" << endl;
            string choice = "0";
            cin >> choice;

            while (choice != "1" && choice != "2") {
                cout << "Please insert a valid frame number." << endl;
                cout << "1- simple frame" << endl;
                cout << "2- fancy frame" << endl;
                cin >> choice;
            }

            if (choice == "1") {
                Image image2(image.width + 2 * image.width / 50, image.height + 2 * image.height / 50);

                for (int i = 0; i < image2.width; i++) {
                    for (int j = 0; j < image2.height; j++) {
                        if (color == "1") {
                            image2(i, j, 0) = 0;
                            image2(i, j, 1) = 0;
                            image2(i, j, 2) = 0;
                        }
                        else if (color == "2") {
                            image2(i, j, 0) = 150;
                            image2(i, j, 1) = 75;
                            image2(i, j, 2) = 0;
                        }
                        else if (color == "3") {
                            image2(i, j, 0) = 255;
                            image2(i, j, 1) = 255;
                            image2(i, j, 2) = 255;
                        }
                        else if (color == "4") {
                            image2(i, j, 0) = 255;
                            image2(i, j, 1) = 0;
                            image2(i, j, 2) = 0;
                        }
                        else if (color == "5") {
                            image2(i, j, 0) = 0;
                            image2(i, j, 1) = 255;
                            image2(i, j, 2) = 0;
                        }
                        else if (color == "6") {
                            image2(i, j, 0) = 0;
                            image2(i, j, 1) = 0;
                            image2(i, j, 2) = 255;
                        }
                        else if (color == "7") {
                            image2(i, j, 0) = 255;
                            image2(i, j, 1) = 255;
                            image2(i, j, 2) = 0;
                        }
                        else if (color == "8") {
                            image2(i, j, 0) = 255;
                            image2(i, j, 1) = 160;
                            image2(i, j, 2) = 0;
                        }
                        else if (color == "9") {
                            image2(i, j, 0) = 160;
                            image2(i, j, 1) = 32;
                            image2(i, j, 2) = 240;
                        }
                    }
                }

                for (int i = 0; i < image.width; i++) {
                    for (int j = 0; j < image.height; j++) {
                        image2(i + image.width / 50, j + image.height / 50, 0) = image(i, j, 0);
                        image2(i + image.width / 50, j + image.height / 50, 1) = image(i, j, 1);
                        image2(i + image.width / 50, j + image.height / 50, 2) = image(i, j, 2);
                    }
                }

                while (true) {
                    cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                    cin >> filename;


                    if (filename.size() < 4 || !isValidExtension(filename)) {
                        cout << "Invalid filename or extension. Please try again." << endl;
                        continue;
                    }

                    break;
                }
                image2.saveImage(filename);

                cout << "Your pic has been filtered successfully and it is saved as : " << filename << endl;

            }


            else if (choice == "2") {
                Image image2(image.width + 2 * image.width / 25, image.height + 2 * image.height / 25);

                cout << "select your favourite design" << endl;
                cout << "1- design 1" << endl;
                cout << "2- design 2" << endl;
                cout << "3- design 3" << endl;
                cout << "4- design 4" << endl;
                cin >> choice;

                while (choice != "1" && choice != "2" && choice != "3" && choice != "4")
                {
                    cout << "Plaese enter a valid design number" << endl;
                    cout << "1- design 1" << endl;
                    cout << "2- design 2" << endl;
                    cout << "3- design 3" << endl;
                    cout << "4- design 4" << endl;
                    cin >> choice;
                }

                if (choice == "1") {

                    for (int i = 0; i < image.width; i++) {
                        for (int j = 0; j < image.height; j++) {
                            image2(i + image.width / 25, j + image.height / 25, 0) = image(i, j, 0);
                            image2(i + image.width / 25, j + image.height / 25, 1) = image(i, j, 1);
                            image2(i + image.width / 25, j + image.height / 25, 2) = image(i, j, 2);
                        }
                    }

                    for (int i = 0; i < image2.width; i++) {
                        for (int j = 0; j < image2.height; j++) {
                            if (i < image.width / 250 || j < image.height / 250 || i > image2.width - image.width / 250 || j > image2.height - image.height / 250) {
                                image2(i, j, 0) = 255;
                                image2(i, j, 1) = 255;
                                image2(i, j, 2) = 255;
                            }
                            else if ((i > 9 * image.width / 250 && i < image.width / 25) && (j >= image.height / 25 && j <= image2.height - image.height / 25) || (i < image2.width - 9 * image.width / 250 && i > image2.width - image.width / 25) && (j >= image.height / 25 && j <= image2.height - image.height / 25) || (j > 9 * image.height / 250 && j < image.height / 25) && (i >= image.width / 25 && i <= image2.width - image.width / 25) || (j < image2.height - 9 * image.height / 250 && j > image2.height - image.height / 25) && (i >= image.width / 25 && i <= image2.width - image.width / 25)) {
                                image2(i, j, 0) = 255;
                                image2(i, j, 1) = 255;
                                image2(i, j, 2) = 255;
                            }
                            else if (i < image.width / 25 || i > image2.width - image.width / 25 || j < image.height / 25 || j > image2.height - image.height / 25) {
                                if (color == "1") {
                                    image2(i, j, 0) = 0;
                                    image2(i, j, 1) = 0;
                                    image2(i, j, 2) = 0;
                                }
                                else if (color == "2") {
                                    image2(i, j, 0) = 150;
                                    image2(i, j, 1) = 75;
                                    image2(i, j, 2) = 0;
                                }
                                else if (color == "3") {
                                    image2(i, j, 0) = 255;
                                    image2(i, j, 1) = 255;
                                    image2(i, j, 2) = 255;
                                }
                                else if (color == "4") {
                                    image2(i, j, 0) = 255;
                                    image2(i, j, 1) = 0;
                                    image2(i, j, 2) = 0;
                                }
                                else if (color == "5") {
                                    image2(i, j, 0) = 0;
                                    image2(i, j, 1) = 255;
                                    image2(i, j, 2) = 0;
                                }
                                else if (color == "6") {
                                    image2(i, j, 0) = 0;
                                    image2(i, j, 1) = 0;
                                    image2(i, j, 2) = 255;
                                }
                                else if (color == "7") {
                                    image2(i, j, 0) = 255;
                                    image2(i, j, 1) = 255;
                                    image2(i, j, 2) = 0;
                                }
                                else if (color == "8") {
                                    image2(i, j, 0) = 255;
                                    image2(i, j, 1) = 160;
                                    image2(i, j, 2) = 0;
                                }
                                else if (color == "9") {
                                    image2(i, j, 0) = 160;
                                    image2(i, j, 1) = 32;
                                    image2(i, j, 2) = 240;
                                }
                            }
                        }
                    }
                }
                else if (choice == "2") {

                    for (int i = 0; i < image2.width; i++) {
                        for (int j = 0; j < image2.height; j++) {

                            if (i < image.width / 50 || j < image.height / 50 || i > image2.width - image.width / 50 || j > image2.height - image.height / 50) {

                                if (color == "1") {
                                    image2(i, j, 0) = 0;
                                    image2(i, j, 1) = 0;
                                    image2(i, j, 2) = 0;
                                }
                                else if (color == "2") {
                                    image2(i, j, 0) = 150;
                                    image2(i, j, 1) = 75;
                                    image2(i, j, 2) = 0;
                                }
                                else if (color == "3") {
                                    image2(i, j, 0) = 255;
                                    image2(i, j, 1) = 255;
                                    image2(i, j, 2) = 255;
                                }
                                else if (color == "4") {
                                    image2(i, j, 0) = 255;
                                    image2(i, j, 1) = 0;
                                    image2(i, j, 2) = 0;
                                }
                                else if (color == "5") {
                                    image2(i, j, 0) = 0;
                                    image2(i, j, 1) = 255;
                                    image2(i, j, 2) = 0;
                                }
                                else if (color == "6") {
                                    image2(i, j, 0) = 0;
                                    image2(i, j, 1) = 0;
                                    image2(i, j, 2) = 255;
                                }
                                else if (color == "7") {
                                    image2(i, j, 0) = 255;
                                    image2(i, j, 1) = 255;
                                    image2(i, j, 2) = 0;
                                }
                                else if (color == "8") {
                                    image2(i, j, 0) = 255;
                                    image2(i, j, 1) = 160;
                                    image2(i, j, 2) = 0;
                                }
                                else if (color == "9") {
                                    image2(i, j, 0) = 160;
                                    image2(i, j, 1) = 32;
                                    image2(i, j, 2) = 240;
                                }
                            }

                            else if (i > image.width / 50 && i < image.width / 25 || j > image.height / 50 && j < image.height / 25 || i > image2.width - image.width / 25 && i < image2.width - image.width / 50 || j > image2.height - image.height / 25 && j < image2.height - image.height / 50) {
                                image2(i, j, 0) = 255;
                                image2(i, j, 1) = 255;
                                image2(i, j, 2) = 255;
                            }
                        }
                    }

                    for (int i = 0; i < image.width; i++) {
                        for (int j = 0; j < image.height; j++) {
                            image2(i + image.width / 25, j + image.height / 25, 0) = image(i, j, 0);
                            image2(i + image.width / 25, j + image.height / 25, 1) = image(i, j, 1);
                            image2(i + image.width / 25, j + image.height / 25, 2) = image(i, j, 2);
                        }
                    }
                }
                else if (choice == "3") {
                    for (int i = 0; i < image2.width; i++) {
                        for (int j = 0; j < image2.height; j++) {
                            if (i < 4 * image.width / 25 && j < image.height / 25 || i < 4 * image.width / 25 && j > image2.height - image.height / 25) {
                                image2(i, j, 0) = 255;
                                image2(i, j, 1) = 255;
                                image2(i, j, 2) = 255;
                            }
                            else if (i < image.width / 25 && j < 4 * image.height / 25 || i < image.width / 25 && j > image2.height - 4 * image.height / 25) {
                                image2(i, j, 0) = 255;
                                image2(i, j, 1) = 255;
                                image2(i, j, 2) = 255;
                            }
                            else if (i > image2.width - 4 * image.width / 25 && j < image.height / 25 || i > image2.width - 4 * image.width / 25 && j > image2.height - image.height / 25) {
                                image2(i, j, 0) = 255;
                                image2(i, j, 1) = 255;
                                image2(i, j, 2) = 255;
                            }
                            else if (i > image2.width - image.width / 25 && j < 4 * image.height / 25 || i > image2.width - image.width / 25 && j > image2.height - 4 * image.height / 25) {
                                image2(i, j, 0) = 255;
                                image2(i, j, 1) = 255;
                                image2(i, j, 2) = 255;
                            }
                            else if (color == "1") {
                                image2(i, j, 0) = 0;
                                image2(i, j, 1) = 0;
                                image2(i, j, 2) = 0;
                            }
                            else if (color == "2") {
                                image2(i, j, 0) = 150;
                                image2(i, j, 1) = 75;
                                image2(i, j, 2) = 0;
                            }
                            else if (color == "3") {
                                image2(i, j, 0) = 255;
                                image2(i, j, 1) = 255;
                                image2(i, j, 2) = 255;
                            }
                            else if (color == "4") {
                                image2(i, j, 0) = 255;
                                image2(i, j, 1) = 0;
                                image2(i, j, 2) = 0;
                            }
                            else if (color == "5") {
                                image2(i, j, 0) = 0;
                                image2(i, j, 1) = 255;
                                image2(i, j, 2) = 0;
                            }
                            else if (color == "6") {
                                image2(i, j, 0) = 0;
                                image2(i, j, 1) = 0;
                                image2(i, j, 2) = 255;
                            }
                            else if (color == "7") {
                                image2(i, j, 0) = 255;
                                image2(i, j, 1) = 255;
                                image2(i, j, 2) = 0;
                            }
                            else if (color == "8") {
                                image2(i, j, 0) = 255;
                                image2(i, j, 1) = 160;
                                image2(i, j, 2) = 0;
                            }
                            else if (color == "9") {
                                image2(i, j, 0) = 160;
                                image2(i, j, 1) = 32;
                                image2(i, j, 2) = 240;
                            }
                        }
                    }

                    for (int i = 0; i < image.width; i++) {
                        for (int j = 0; j < image.height; j++) {
                            image2(i + image.width / 25, j + image.height / 25, 0) = image(i, j, 0);
                            image2(i + image.width / 25, j + image.height / 25, 1) = image(i, j, 1);
                            image2(i + image.width / 25, j + image.height / 25, 2) = image(i, j, 2);
                        }
                    }
                }
                else if (choice == "4") {
                    for (int i = 0; i < image2.width; i++) {
                        for (int j = 0; j < image2.height; j++) {
                            if (i < image.width / 25 && j < image.height / 25 || i < image.width / 25 && j > image2.height - image.height / 25) {
                                image2(i, j, 0) = 255;
                                image2(i, j, 1) = 255;
                                image2(i, j, 2) = 255;
                            }
                            else if (i > image2.width - image.width / 25 && j < image.height / 25 || i > image2.width - image.width / 25 && j > image2.height - image.height / 25) {
                                image2(i, j, 0) = 255;
                                image2(i, j, 1) = 255;
                                image2(i, j, 2) = 255;
                            }
                            else if (i > image.width / 100 && i < 3 * image.width / 100 || i < image2.width - image.width / 100 && i > image2.width - 3 * image.width / 100) {
                                image2(i, j, 0) = 255;
                                image2(i, j, 1) = 255;
                                image2(i, j, 2) = 255;
                            }
                            else if (j > image.height / 100 && j < 3 * image.height / 100 || j < image2.height - image.height / 100 && j > image2.height - 3 * image.height / 100) {
                                image2(i, j, 0) = 255;
                                image2(i, j, 1) = 255;
                                image2(i, j, 2) = 255;
                            }
                            else if (color == "1") {
                                image2(i, j, 0) = 0;
                                image2(i, j, 1) = 0;
                                image2(i, j, 2) = 0;
                            }
                            else if (color == "2") {
                                image2(i, j, 0) = 150;
                                image2(i, j, 1) = 75;
                                image2(i, j, 2) = 0;
                            }
                            else if (color == "3") {
                                image2(i, j, 0) = 255;
                                image2(i, j, 1) = 255;
                                image2(i, j, 2) = 255;
                            }
                            else if (color == "4") {
                                image2(i, j, 0) = 255;
                                image2(i, j, 1) = 0;
                                image2(i, j, 2) = 0;
                            }
                            else if (color == "5") {
                                image2(i, j, 0) = 0;
                                image2(i, j, 1) = 255;
                                image2(i, j, 2) = 0;
                            }
                            else if (color == "6") {
                                image2(i, j, 0) = 0;
                                image2(i, j, 1) = 0;
                                image2(i, j, 2) = 255;
                            }
                            else if (color == "7") {
                                image2(i, j, 0) = 255;
                                image2(i, j, 1) = 255;
                                image2(i, j, 2) = 0;
                            }
                            else if (color == "8") {
                                image2(i, j, 0) = 255;
                                image2(i, j, 1) = 160;
                                image2(i, j, 2) = 0;
                            }
                            else if (color == "9") {
                                image2(i, j, 0) = 160;
                                image2(i, j, 1) = 32;
                                image2(i, j, 2) = 240;
                            }
                        }
                    }

                    for (int i = 0; i < image.width; i++) {
                        for (int j = 0; j < image.height; j++) {
                            image2(i + image.width / 25, j + image.height / 25, 0) = image(i, j, 0);
                            image2(i + image.width / 25, j + image.height / 25, 1) = image(i, j, 1);
                            image2(i + image.width / 25, j + image.height / 25, 2) = image(i, j, 2);
                        }
                    }
                }

                while (true) {
                    cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                    cin >> filename;


                    if (filename.size() < 4 || !isValidExtension(filename)) {
                        cout << "Invalid filename or extension. Please try again." << endl;
                        continue;
                    }

                    break;
                }
                image2.saveImage(filename);

                cout << "Your pic has been filtered successfully and it is saved as : " << filename << endl;

            }

        }

        else if (option == "10")
        {
            string img1, outimg;
            // Input image validation loop
            while (true) {
                cout << "Enter your image please: ";
                cin >> img1;

                if (img1.size() < 4 || !isValidExtension(img1)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                // Check if the file exists
                if (!fileExists(img1)) {
                    cout << "The file '" << img1 << "' does not exist. Please try again." << endl;
                    continue;
                }

                break;
            }

            // Output image validation loop
            while (true) {
                cout << "Enter the name of the output image with the correct extension (.jpg, .bmp, .png, .tga): ";
                cin >> outimg;

                if (outimg.size() < 4 || !isValidExtension(outimg)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                break;
            }

            Image inputImage(img1);
            Image outputImage(inputImage.width, inputImage.height);

            applySobel(inputImage, outputImage);

            outputImage.saveImage(outimg);
            cout << "Edge detection completed successfully. Output saved to: " << outimg << endl;


        }

        else if (option == "11")
        {

            while (true) {
                cout << "Please enter the image name with its extension: ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                // Check if the file exists
                if (!fileExists(filename)) {
                    cout << "The file '" << filename << "' does not exist. Please try again." << endl;
                    continue;
                }

                break;
            }

            Image image(filename);
            int newWidth, newHeight;
            string userValue1 = "0", userValue2 = "0";
            cout << "Enter the new width and height: ";
            cin >> userValue1 >> userValue2;

            while (!isnumbers(userValue1, userValue2) || stoi(userValue1) > 10000 || stoi(userValue2) > 10000 || stoi(userValue1) == 0 || stoi(userValue2) == 0) {

                if (!isnumbers(userValue1, userValue2)) {
                    cout << "Please enter a valid dimensions" << endl;
                    cin >> userValue1 >> userValue2;
                }

                else {
                    cout << "Size limit exceeded please enter the new dimensions agine" << endl;
                    cin >> userValue1 >> userValue2;
                }
            }

            newWidth = stof(userValue1);
            newHeight = stof(userValue2);
            Image newImage(newWidth, newHeight);
            double scaleX = static_cast<double>(image.width) / newWidth;
            double scaleY = static_cast<double>(image.height) / newHeight;

            for (int i = 0; i < newWidth; ++i) {
                for (int j = 0; j < newHeight; ++j) {
                    int origX = round(i * scaleX);
                    int origY = round(j * scaleY);
                    for (int k = 0; k < 3; k++)
                    {
                        newImage(i, j, k) = image(origX, origY, k);
                    }
                }
            }

            while (true) {
                cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                break;
            }
            newImage.saveImage(filename);
        }


        else if (option == "12")
        {

            while (true) {
                cout << "Please enter the image name with its extension: ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                // Check if the file exists
                if (!fileExists(filename)) {
                    cout << "The file '" << filename << "' does not exist. Please try again." << endl;
                    continue;
                }

                break;
            }
            Image image(filename);

            string userInput;
            cout << "Enter the blur quality you want, it must be a positive, odd number >= 3" << endl;
            cin >> userInput;

            while (!isValidInput(userInput)) {
                cout << "Invalid, blur quality must be be a positive, odd number >= 3" << endl;
                cin >> userInput;
            }

            int kernelSize = stoi(userInput);

            for (int i = 0; i < image.width; i++) {
                for (int j = 0; j < image.height; j++) {

                    int redSum = 0, greenSum = 0, blueSum = 0;

                    for (int k = -kernelSize / 2; k <= kernelSize / 2; k++) {
                        for (int l = -kernelSize / 2; l <= kernelSize / 2; l++) {

                            int horizontalNeighbor = k + i;
                            int verticalNeighbor = l + j;

                            if (horizontalNeighbor >= 0 && horizontalNeighbor < image.width && verticalNeighbor >= 0 && verticalNeighbor < image.height) {
                                redSum += image(horizontalNeighbor, verticalNeighbor, 0);
                                greenSum += image(horizontalNeighbor, verticalNeighbor, 1);
                                blueSum += image(horizontalNeighbor, verticalNeighbor, 2);
                            }
                        }
                    }

                    image(i, j, 0) = redSum / (kernelSize * kernelSize);
                    image(i, j, 1) = greenSum / (kernelSize * kernelSize);
                    image(i, j, 2) = blueSum / (kernelSize * kernelSize);
                }
            }

            while (true) {
                cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                break;
            }
            image.saveImage(filename);

        }

        else if (option == "13")
        {

            while (true) {
                cout << "Please enter the image name with its extension: ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                // Check if the file exists
                if (!fileExists(filename)) {
                    cout << "The file '" << filename << "' does not exist. Please try again." << endl;
                    continue;
                }

                break;
            }
            Image image(filename);

            for (int i = 0; i < image.width; i++)
            {
                for (int j = 0; j < image.height; j++)
                {
                    unsigned char& red = image(i, j, 0);
                    unsigned char& green = image(i, j, 1);
                    unsigned char& blue = image(i, j, 2);
                    red = min(red + 30, 255);
                    green = min(green + 20, 255);
                    blue = max(blue - 50, 0);
                }
            }

            while (true) {
                cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                break;
            }
            image.saveImage(filename);


        }

        else if (option == "14") {
            while (true) {
                cout << "Please enter the image name with its extension: ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                // Check if the file exists
                if (!fileExists(filename)) {
                    cout << "The file '" << filename << "' does not exist. Please try again." << endl;
                    continue;
                }

                break;
            }
            Image image(filename);
            int radius = 2, intensitynLevel = 4;

            for (int i = 0; i < image.width; i++) {
                for (int j = 0; j < image.height; j++) {

                    int red_avg[20] = { 0 }, green_avg[20] = { 0 }, blue_avg[20] = { 0 }, intensitycount[20] = { 0 };

                    for (int k = -radius; k <= radius; k++) {
                        for (int l = -radius; l <= radius; l++) {
                            int horizontalNeighbor = k + i;
                            int verticalNeighbor = l + j;
                            if (horizontalNeighbor >= 0 && horizontalNeighbor < image.width && verticalNeighbor >= 0 && verticalNeighbor < image.height) {

                                int currentIntesity = (image(horizontalNeighbor, verticalNeighbor, 0) + image(horizontalNeighbor, verticalNeighbor, 1) + image(horizontalNeighbor, verticalNeighbor, 2)) / 3 * intensitynLevel / 255;
                                intensitycount[currentIntesity]++;
                                red_avg[currentIntesity] += image(horizontalNeighbor, verticalNeighbor, 0);
                                green_avg[currentIntesity] += image(horizontalNeighbor, verticalNeighbor, 1);
                                blue_avg[currentIntesity] += image(horizontalNeighbor, verticalNeighbor, 2);
                            }
                        }
                    }

                    int maxCount = 0;
                    int maxIndex = 0;
                    for (int i = 0; i < intensitynLevel; i++) {
                        if (intensitycount[i] > maxCount) {
                            maxCount = intensitycount[i];
                            maxIndex = i;
                        }
                    }

                    image(i, j, 0) = red_avg[maxIndex] / maxCount;
                    image(i, j, 1) = green_avg[maxIndex] / maxCount;
                    image(i, j, 2) = blue_avg[maxIndex] / maxCount;
                }
            }

            while (true) {
                cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                break;
            }
            image.saveImage(filename);

            cout << "Your pic has been filtered successfully and it is saved as : " << filename << endl;


        }

        else if (option == "15")
        {
            while (true) {
                cout << "Please enter the image name with its extension: ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                // Check if the file exists
                if (!fileExists(filename)) {
                    cout << "The file '" << filename << "' does not exist. Please try again." << endl;
                    continue;
                }

                break;
            }
            Image image(filename);
            // Add noise to simulate old TV distortion
            addNoise(image, 20); // Adjust intensity as needed

            while (true) {
                cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                break;
            }
            image.saveImage(filename);

            cout << "Your pic has been filtered successfully and it is saved as : " << filename << endl;

        }

        else if (option == "16")
        {
            while (true) {
                cout << "Please enter the image name with its extension: ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                // Check if the file exists
                if (!fileExists(filename)) {
                    cout << "The file '" << filename << "' does not exist. Please try again." << endl;
                    continue;
                }

                break;
            }
            Image image(filename);
            applyPurpleFilter(image); // Apply the purple filter

            while (true) {
                cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                break;
            }
            image.saveImage(filename);

            cout << "Your pic has been filtered successfully and it is saved as : " << filename << endl;
        }

        else if (option == "17")
        {
            while (true) {
                cout << "Please enter the image name with its extension: ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                // Check if the file exists
                if (!fileExists(filename)) {
                    cout << "The file '" << filename << "' does not exist. Please try again." << endl;
                    continue;
                }

                break;
            }

            Image image(filename);

            for (int i = 0; i < image.width; i++) {
                for (int j = 0; j < image.height; j++) {
                    image(i, j, 0) = 255;
                    image(i, j, 1) = 255 - image(i, j, 1);
                    image(i, j, 2) = 255 - image(i, j, 2);
                }
            }

            while (true) {
                cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                break;
            }
            image.saveImage(filename);

            cout << "Your pic has been filtered successfully and it is saved as : " << filename << endl;

        }

        else if (option == "18")
        {
            while (true) {
                cout << "Please enter the image name with its extension: ";
                cin >> filename;

                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                // Check if the file exists
                if (!fileExists(filename)) {
                    cout << "The file '" << filename << "' does not exist. Please try again." << endl;
                    continue;
                }

                break;
            }
            Image image(filename);
            string choice = "0";

            cout << "Enter the number of skew you want" << endl;
            cout << "1- skew to right" << endl;
            cout << "2- skew to left" << endl;
            cin >> choice;

            while (choice != "1" && choice != "2") {
                cout << "Please enter a valid skew number" << endl;
                cout << "1- skew to right" << endl;
                cout << "2- skew to left" << endl;
                cin >> choice;
            }

            double skewDegree = 0;
            string userValue = "0";
            cout << "Please enter the skew degree: ";
            cin >> userValue;


            while (!isValidDegree(userValue) || image.width + int(image.height * tan(stof(userValue) * 3.141592654 / 180)) > 136056)
            {
                if (!isValidDegree(userValue))
                {
                    cout << "Please enter a valid skew degree: ";
                    cin >> userValue;
                }
                else {
                    cout << "Size limit exceeded please enter smaller skew degree: ";
                    cin >> userValue;
                }
            }

            skewDegree = stof(userValue);
            Image image2(image.width + int(image.height * tan(skewDegree * 3.141592654 / 180)), image.height);

            for (int i = 0; i < image2.width; i++) {
                for (int j = 0; j < image2.height; j++) {
                    image2(i, j, 0) = 255;
                    image2(i, j, 1) = 255;
                    image2(i, j, 2) = 255;
                }
            }

            if (choice == "1") {
                for (int i = 0; i < image.height; i++) {
                    for (int j = 0; j < image.width; j++) {
                        int skewDistance = 0;
                        if (skewDegree >= 0) {
                            skewDistance = j + int((image.height - 1 - i) * tan(skewDegree * 3.141592654 / 180));
                        }
                        else {
                            skewDistance = j + int(i * tan(skewDegree * 3.141592654 / 180));
                        }if (skewDistance >= 0 && skewDistance < image2.width) {
                            image2(skewDistance, i, 0) = image(j, i, 0);
                            image2(skewDistance, i, 1) = image(j, i, 1);
                            image2(skewDistance, i, 2) = image(j, i, 2);
                        }
                    }
                }
            }

            else {
                for (int i = 0; i < image.height; i++) {
                    for (int j = 0; j < image.width; j++) {
                        int skewDistance = 0;
                        skewDistance = j + int(i * tan(skewDegree * 3.141592654 / 180));
                        if (skewDistance >= 0 && skewDistance < image2.width) {
                            image2(skewDistance, i, 0) = image(j, i, 0);
                            image2(skewDistance, i, 1) = image(j, i, 1);
                            image2(skewDistance, i, 2) = image(j, i, 2);
                        }
                    }
                }
            }

            while (true) {
                cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                break;
            }
            image2.saveImage(filename);

            cout << "Your pic has been filtered successfully and it is saved as : " << filename << endl;

        }

        else if (option == "19")
        {
            while (true) {
                cout << "Please enter the image name with its extension: ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                // Check if the file exists
                if (!fileExists(filename)) {
                    cout << "The file '" << filename << "' does not exist. Please try again." << endl;
                    continue;
                }

                break;
            }
            Image image(filename);
            for (int y = 0; y < image.height; ++y) {
                for (int x = 0; x < image.width; ++x) {

                    unsigned char& r = image.getPixel(x, y, 0);
                    unsigned char& g = image.getPixel(x, y, 1);
                    unsigned char& b = image.getPixel(x, y, 2);
                    r = min(255, static_cast<int>(r * 1.25));
                    g = min(255, static_cast<int>(r * 0.75));
                    b = min(255, static_cast<int>(r * 1.25));
                }
            }


            while (true) {
                cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                break;
            }
            image.saveImage(filename);

            cout << "Your pic has been filtered successfully and it is saved as : " << filename << endl;

        }

        else if (option == "20")
        {

            while (true) {
                cout << "Please enter the image name with its extension: ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                // Check if the file exists
                if (!fileExists(filename)) {
                    cout << "The file '" << filename << "' does not exist. Please try again." << endl;
                    continue;
                }

                break;
            }
            Image image(filename);

            // Apply mirror effect
            applyMirrorEffect(image);

            while (true) {
                cout << "Please enter the output image name with the correct extension (.jpg, .bmp, .png, .tga): ";
                cin >> filename;


                if (filename.size() < 4 || !isValidExtension(filename)) {
                    cout << "Invalid filename or extension. Please try again." << endl;
                    continue;
                }

                break;
            }
            image.saveImage(filename);

            cout << "Your pic has been filtered successfully and it is saved as : " << filename << endl;

        }

        else if (option == "21")
        {
            break;
        }
    }

}