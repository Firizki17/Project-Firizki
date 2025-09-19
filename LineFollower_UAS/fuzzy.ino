// Fungsi Fuzzy Control
void fuzzyControl(int distance, int position) {
  float near = trapezoid(distance, 0, 0, 10, 20);
  float medium = triangle(distance, 10, 25, 40);
  float far = trapezoid(distance, 30, 50, 100, 100);

  float pos_left = (position == -1) ? 1 : 0;
  float pos_center = (position == 0) ? 1 : 0;
  float pos_right = (position == 1) ? 1 : 0;

  float slow = 0, mediumSpeed = 0, fast = 0;
  float pwmL = 0, pwmR = 0, sumL = 0, sumR = 0;

  float rule1 = near;
  slow += rule1;

  float rule2 = min(medium, pos_center);
  mediumSpeed += rule2;

  float rule3 = min(medium, pos_left);
  mediumSpeed += rule3;

  float rule4 = min(medium, pos_right);
  mediumSpeed += rule4;

  float rule5 = min(far, pos_center);
  fast += rule5;

  float rule6 = min(far, pos_left);
  pwmL += rule6 * 100;
  pwmR += rule6 * 255;
  sumL += rule6;
  sumR += rule6;

  float rule7 = min(far, pos_right);
  pwmL += rule7 * 255;
  pwmR += rule7 * 100;
  sumL += rule7;
  sumR += rule7;

  float defuzzL = (slow*0 + mediumSpeed*150 + fast*255);
  float defuzzR = (slow*0 + mediumSpeed*150 + fast*255);
  float total = slow + mediumSpeed + fast;

  if (total > 0) {
    defuzzL /= total;
    defuzzR /= total;
  }
  if (sumL > 0) defuzzL = (pwmL / sumL);
  if (sumR > 0) defuzzR = (pwmR / sumR);

  setMotor(defuzzL, defuzzR);
}

float triangle(int x, int a, int b, int c) {
  if (x <= a || x >= c) return 0;
  else if (x == b) return 1;
  else if (x < b) return float(x - a) / (b - a);
  else return float(c - x) / (c - b);
}

float trapezoid(int x, int a, int b, int c, int d) {
  if (x <= a || x >= d) return 0;
  else if (x >= b && x <= c) return 1;
  else if (x > a && x < b) return float(x - a) / (b - a);
  else return float(d - x) / (d - c);
}
