R = csvread('../build/Debug/retina.csv')
F = csvread('../build/Debug/spatial.csv')
S = csvread('../build/Debug/sum.csv')


%{
1. Gernrate map w/ normal distribution (across fovea & parafovea)
2. Add stimulus submarticies
3. Do convolution with 3x3 gaussian kernel
%}

figure
surf(R)
axis vis3d %lock aspect ratio
colorbar

figure
surf(F)
axis vis3d %lock aspect ratio
colorbar

figure
surf(S)
axis vis3d %lock aspect ratio
colorbar

%{
colormap('jet')
imagesc(S)
colorbar
%}