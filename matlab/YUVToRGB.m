function [rgbImg] = YUVToRGB(yuvImg, yuvRows, yuvCols)
% YUVToRGB Convert a yuv byte array image into a 3 channel rgb image

yuvImg = reshape(yuvImg, [], yuvRows)';
% Extract Y, U, and V components
Y = yuvImg(:, 1:2:end);
U = yuvImg(:, 2:4:end);
V = yuvImg(:, 4:4:end);

% Upsample U and V to match Y size
U = repelem(U, 1, 2);
V = repelem(V, 1, 2);

% Pre-allocate the RGB matrix
rgbImg = zeros(yuvRows, yuvCols, 3, 'uint8');

% Convert to RGB
R = Y + 1.402 * (V - 128);
G = Y - 0.344136 * (U - 128) - 0.714136 * (V - 128);
B = Y + 1.772 * (U - 128);

% Clip the values to be in the uint8 range [0, 255]
R = uint8(max(min(R, 255), 0));
G = uint8(max(min(G, 255), 0));
B = uint8(max(min(B, 255), 0));

% Combine R, G, and B into one 3D matrix
rgbImg(:, :, 1) = R;
rgbImg(:, :, 2) = G;
rgbImg(:, :, 3) = B;

end

