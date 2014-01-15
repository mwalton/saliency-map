A = csvread('../Build/saliency_map/Build/Products/Debug/map_test.csv')

%{
1. Gernrate map w/ normal distribution (across fovea & parafovea)
2. Add stimulus submarticies
3. Do convolution with 3x3 gaussian kernel
%}

figure
surf(A)
axis vis3d %lock aspect ratio
colorbar