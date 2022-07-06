int main() {
	int a = 4 + 5;
	int c = 4 + 7 + 10 - 10 * 2;
	int b;
	b = 10;
	b = a * c + a * c;
	int i = 0; 
	while (i < 10) {
		int d = 1;
		i += d;
	}
	a = i*2;
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
	int e = a * 1 + b * 2 + c * 3 + 10 * 2 + i*2;
}
