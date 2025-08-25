% Moving Average Filter Tradeoff Demo
clear; close all; clc;

% Generate a noisy signal
N = 500;                       % Signal length
t = linspace(0, 2*pi, N);
signal = sin(5*t) + 0.5*randn(1,N); % Sine wave + noise

% Define different moving average window sizes to compare
windowSizes = [5, 25, 75];  

% Apply moving average filters
filteredSignals = cell(length(windowSizes),1);
for k = 1:length(windowSizes)
    b = (1/windowSizes(k)) * ones(1,windowSizes(k)); % FIR filter coefficients
    filteredSignals{k} = filter(b, 1, signal);
end

% --- Plot time-domain results ---
figure;
subplot(2,1,1);
plot(t, signal, 'k', 'DisplayName','Noisy Signal');
hold on;
colors = lines(length(windowSizes));
for k = 1:length(windowSizes)
    plot(t, filteredSignals{k}, 'LineWidth', 1.5, ...
         'DisplayName', sprintf('MA Filter, L=%d', windowSizes(k)), ...
         'Color', colors(k,:));
end
xlabel('Time'); ylabel('Amplitude');
title('Effect of Moving Average Length in Time Domain');
legend show; grid on;

% --- Plot frequency responses ---
subplot(2,1,2);
for k = 1:length(windowSizes)
    b = (1/windowSizes(k)) * ones(1,windowSizes(k));
    [H, f] = freqz(b, 1, 1024, 'half'); % Frequency response
    plot(f, 20*log10(abs(H)), 'LineWidth', 1.5, ...
        'DisplayName', sprintf('L=%d', windowSizes(k)));
    hold on;
end
xlabel('Normalized Frequency (\times\pi rad/sample)');
ylabel('Magnitude (dB)');
title('Frequency Response of Moving Average Filters');
legend show; grid on;

