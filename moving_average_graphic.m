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
plot(t, signal, 'b');
title('Original Signal');
xlabel('Time (s)');
ylabel('Amplitude');
grid on;

subplot(1,2,2);
plot(t, filteredSignal, 'r');
title('After Moving Average Filter');
xlabel('Time (s)');
ylabel('Amplitude');
grid on;

% Optional: match y-axis limits for direct comparison
yLimits = [-2 2];
subplot(1,2,1); ylim(yLimits);
subplot(1,2,2); ylim(yLimits);

% Export the figure as an image 
exportgraphics(gcf, 'moving_average_demo.png', 'Resolution', 300);

