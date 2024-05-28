function [pngImg] = LoadPNGImage(pathToFile)
%LOADPNGIMAGE Merely a wrapper around imread()
% I wrapped this function because we have tests around the functionality of
% loading images and will need to recreate it when porting to C
pngImg = imread(pathToFile);

end

