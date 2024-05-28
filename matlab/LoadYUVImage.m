function [rows, cols, data] = LoadYUVImage(pathToFile)
%LOADYUVIMAGE Load a YUV binary file into memory.
% No reshaping or resizing of the data is done, so the return data
% will simply be an array of bytes.
% Inputs:
%   pathToFile: /some/path/to/a/yuv.bin
% Returns:
%   rows, cols, data

data = [];
rows = 0;
cols = 0;
fid = fopen(pathToFile, 'rb');
if fid < 0
    return;
end


% Read the number of cols. In the cerberus data logger the first 4 bytes
% is the expected number of cols in the image
cols = fread(fid, 1, 'int32');
if isempty(cols)
    error('Could not read the number of cols.');
end

% Read the number of rows. In the cerberus data logger the nexts 4 bytes
% is the expected number of rows in the image
rows = fread(fid, 1, 'int32');
if isempty(rows)
    error('Could not read the number of rows.');
end

% If we dont put the * here matlab will read in each byte and then convert
% to a double type. The value will be correct, but the type would be wrong 
data = fread(fid, '*uint8');

fclose(fid);
end

