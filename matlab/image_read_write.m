% Helper script to read in image data and write the channel/pixel it to a file
% This was used to generate the true data for CV unit tests
% The data is written as a binary file of [r, g, b, r, g, b, ....] so we can read it in one big
% chunk in c/c++ and not require an external library for reading png/jpg/etc data

%% Generate true RGB image data
img = imread("/home/atc/cerberus/tests/test_data/cv/img_rgb.png");

% Transpose the image to switch rows and columns
% This is done because by default matlab will write the image in a column major order by default
% and we want row major for the unit tests
img_transposed = permute(img, [2, 1, 3]);

[rows, cols, ~] = size(img)

pixel_data = reshape(img, rows * cols * 3, 1);
fileID = fopen('/home/atc/cerberus/tests/test_data/cv/true_rgb.bin', 'w');
fwrite(fileID, pixel_data, 'uint8');
fclose(fileID);

%% Generate true greyscale image data
img = imread("/home/atc/cerberus/tests/test_data/cv/img_grey.png");


[rows, cols, ~] = size(img)

% Transpose the image to switch rows and columns
% This is done because by default matlab will write the image in a column major order by default
% and we want row major for the unit tests
pixel_data = reshape(img', rows * cols * 1, 1);
fileID = fopen('/home/atc/cerberus/tests/test_data/cv/true_grey.bin', 'w');
fwrite(fileID, pixel_data, 'uint8');
fclose(fileID);