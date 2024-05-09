#pragma once
class KalmanFilter {
public:
	KalmanFilter(double process_noise, double measurement_noise, double estimated_error) {
		Q = process_noise;
		R = measurement_noise;
		P = estimated_error;
		
	}

    void setInitValue(double value)
    {
        X = value;
    }

	double update(double measurement) {
		K = P / (P + R);
		X = X + K * (measurement - X);
		P = (1 - K) * P + Q;

		return X;
	}

private:
	double Q; // process noise
	double R; // measurement noise
	double P; // estimated error
	double K; // kalman gain
	double X; // value
};