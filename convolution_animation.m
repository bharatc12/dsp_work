% Example signal and kernel
signal = [0 0 1 2 3 2 1 0 0 0];    % input signal
kernel = [0.2 0.5 0.2];            % convolution kernel
N = length(signal);
M = length(kernel);

% Prepare for convolution output
conv_result = zeros(1, N + M - 1);

figure('Position', [100 100 800 400]);

% Prepare GIF file
gif_filename = 'convolution_demo.gif';

% Loop over each convolution step
for k = 1:(N+M-1)
    % Clear figure
    clf;
    
    % Time axis for plotting
    n_signal = 1:N;
    n_kernel = (1:M) + k - M;  % shift kernel index
    
    % Plot signal
    subplot(2,1,1);
    stem(n_signal, signal, 'b', 'LineWidth', 1.5); hold on;
    
    % Plot shifted kernel 
    valid_idx = (n_kernel >= 1) & (n_kernel <= N);
    stem(n_kernel(valid_idx), kernel(valid_idx), 'r', 'LineWidth', 1.5);
    title(sprintf('Step %d: Sliding Kernel', k));
    xlabel('Sample Index');
    ylabel('Amplitude');
    legend('Signal','Kernel (shifted)');
    grid on;
    ylim([0 max([signal kernel])*1.2]);
    
    % Convolution computation for current step
    conv_result(k) = sum(signal(valid_idx) .* kernel(valid_idx));
    
    % Plot convolution result so far
    subplot(2,1,2);
    stem(1:k, conv_result(1:k), 'k', 'LineWidth', 1.5);
    title('Convolution Output Building Over Time');
    xlabel('Output Sample Index');
    ylabel('Value');
    grid on;
    ylim([min(conv_result)-0.5 max(conv_result)+0.5]);
    
    % Capture frame for GIF
    drawnow;
    frame = getframe(gcf);
    im = frame2im(frame);
    [A, map] = rgb2ind(im, 256);
    if k == 1
        imwrite(A, map, gif_filename, 'gif', 'LoopCount', Inf, 'DelayTime', 0.3);
    else
        imwrite(A, map, gif_filename, 'gif', 'WriteMode', 'append', 'DelayTime', 0.3);
    end
end

