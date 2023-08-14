#include <memory>
#include <iostream>

#define fn auto

fn asd() -> std::shared_ptr<int[]> {
	return std::shared_ptr<int[]>(
		new int[20]
	);
}


fn main() -> int {
	auto b = asd();
	b[1] = 2;
	std::cout << b[1] << std::endl;
	return 0;
}
