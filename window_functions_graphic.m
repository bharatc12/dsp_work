% Example data: noisy sine wave
t = 0:0.01:2*pi;
signal = sin(2*pi*1*t) + 0.3*randn(size(t));

% Moving average filter parameters
windowSize = 10;  % number of points in the average
b = (1/windowSize)*ones(1,windowSize);  % FIR coefficients
a = 1;  % no feedback

% Apply the moving average filter
filteredSignal = filter(b, a, signal);

% Example data: noisy sine wave
t = 0:0.01:2*pi;
signal = sin(2*pi*1*t) + 0.3*randn(size(t));

% Moving average filter parameters
windowSize = 10;  % number of points in the average
b = (1/windowSize)*ones(1,windowSize);  % FIR coefficients
a = 1;  % no feedback

% Apply the moving average filter
filteredSignal = filter(b, a, signal);

% Create figure with side-by-side subplots
figure('Position', [100 100 1000 400]); % wide figure for side-by-side

subplot(1,2,1);
% Number of points in the windows
N = 64;

% Create the window functions
w_blackman = blackman(N);
w_hamming  = hamming(N);

% Create time index for plotting
n = 0:N-1;

% Make figure
figure('Position', [100 100 800 400]);

% Blackman window plot
subplot(1,2,1);
plot(n, w_blackman, 'LineWidth', 2);
title('Blackman Window');
xlabel('Sample');
ylabel('Amplitude');
grid on;
ylim([0 1.1]);

% Hamming window plot
subplot(1,2,2);
plot(n, w_hamming, 'LineWidth', 2);
title('Hamming Window');
xlabel('Sample');
ylabel('Amplitude');
grid on;
ylim([0 1.1]);

% Save figure for HTML use
exportgraphics(gcf, 'window_functions_comparison.png', 'Resolution', 300);

