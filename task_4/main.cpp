#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iostream>

class feature {
public:
	enum FeatureType {
		e_unknown = 0,
		e_circle = 1,
		e_triangle = 2,
		e_square = 3
	};

	feature() : points_{nullptr}, type_{e_unknown} {}

	~feature() {
		clear();
	}

	bool is_valid() const {
		return type_ != e_unknown;
	}

	bool read(FILE* file) {
		if (!file) {
			std::cerr << "File not open for reading.\n";
			return false;
		}

		int type = 0; // Тип фигуры

		if (fscanf_s(file, "%d", &type) != 1) {
			std::cerr << "Unable to read shape type.\n";
			return false;
		}

		type_ = static_cast<FeatureType>(type);

		short n = 0; // количество координат

		switch (type_) {
		case e_circle:
			n = 3;
			break;
		case e_triangle:
			n = 6;
			break;
		case e_square:
			n = 8;
			break;
		case e_unknown:
			std::cerr << "Unknown figure type (" << type << ").\n";
			type_ = e_unknown;
			return false;
		}

		points_ = new double[n];

		if (!points_) {
			std::cerr << "Failed to allocate memory for coordinates.\n";
			type_ = e_unknown;
			return false;
		}

		for (int i = 0; i < n; i++) {
			if (fscanf_s(file, "%lf", &points_[i]) != 1) {
				clear();
				type_ = e_unknown;
				return false;
			}
		}

		return true;
	}

	void draw() const {
		if (!is_valid() || !points_) {
			std::cerr << "Unable to draw shape: data is invalid.\n";
			return;
		}

		switch (type_) {
		case e_circle:
			draw_circle(points_[0], points_[1], points_[2]);
			break;
		case e_triangle:
			draw_polygon(points_, 6);
			break;
		case e_square:
			draw_polygon(points_, 8);
			break;
		case e_unknown:
			std::cerr << "Unknown figure type.\n";
			break;
		}
	}

private:
	void clear() {
		delete[] points_;
		points_ = nullptr;
	}

	// просто заглушки
	void draw_circle(double, double, double) const {
		std::cout << "Success";
	}

	void draw_polygon(const double*, int) const {
		std::cout << "Success";
	}

	double* points_;
	FeatureType type_;
};

// чтобы искать в той же папке, что и main.cpp, нашел в инете
std::filesystem::path local_file(const std::string& name) {
	const std::filesystem::path src_path = __FILE__;
	return src_path.parent_path() / name;
}

int main() {
	feature feat;

	const auto path = local_file("features.dat");

	FILE* file = nullptr;

	if (fopen_s(&file, path.string().c_str(), "r") != 0) {
		std::cerr << "Failed to open file " << path << "\n";
		return 1;
	}

	if (!feat.read(file)) {
		std::cerr << "Reading file failed \n";
		(void)fclose(file);
		return 1;
	}

	(void)fclose(file);
	feat.draw();
	return 0;
}
