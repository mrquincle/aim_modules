samples=4000;

function r = gauss_rand(n, m, mu, sigma)
    % Generate random number based on inverse transform sampling method
    % n: number of rows
    % m: number of columns
    % mu: mean of distribution
    % sigma: standard deviation of distribution

    if nargin < 4
        mu = 0;
        sigma = 1;
    end
    r = sqrt(2) * sigma * erfinv(2 * rand(n, m) - 1) + mu;
end

% generate class id 1
class_id=1;
x_mean = -0.4;
y_mean = 0.4;
x_var = 0.1;
y_var = 0.1;
tz0_x=gauss_rand(samples,1,x_mean,x_var);
tz0_y=gauss_rand(samples,1,x_mean,x_var);
tz0=[tz0_x tz0_y ones(samples,1)*class_id];

% generate class id 2
class_id=2;
x_mean = 0.4;
y_mean = 0.4;
x_var = 0.1;
y_var = 0.1;
tz1_x=gauss_rand(samples,1,x_mean,x_var);
tz1_y=gauss_rand(samples,1,x_mean,x_var);
tz1=[tz1_x tz1_y ones(samples,1)*class_id];

% write to file
file_id = fopen('gaussian.data', 'w');
fdisp(file_id, tz0);
fdisp(file_id, tz1);
fclose(file_id);

% plot, if you want to, uncomment
plot(tz0_x, tz0_y, 'x');
hold on
plot(tz1_x, tz1_y, 'o');
hold off


