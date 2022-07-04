int main() {
	int a = 1, b = 0, c = 20;
	a = 1;
	b = a + c;
	c = 0;
	while (a <= b && a <= 40) {
		a *= 2;
		b = b * 2 - c;
		if (c < 10) {
			++c;
		}
	}
	if (b < 2 || a > 30) {
		b = 4;
	}
	else if (b <= 4) {
		b = c ^ b;
		if (a > b) {
			b = a | b;
		}
		else {
			a = b;
		}
	}
	b <<= 4;
	int d = a * 1 + b * 2 + c * 3;
}