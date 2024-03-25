double std(double arr[], int n) {
    double sum = 0.0;
    double curr_avg = 0.0;

    for (int i = 0; i < n; i++) {
        curr_avg += arr[i];
    }
    curr_avg /= n;

    for (int i = 0; i < n; i++) {
        sum += (arr[i] - curr_avg) * (arr[i] - curr_avg);
    }

    double variation = sum / n;
    return variation;
}