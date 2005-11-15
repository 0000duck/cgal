#ifndef NUAGE_OPTIONS_H
#define NUAGE_OPTIONS_H


namespace NUAGE {

class Options {
public:
    Options()
        :  file_input(true), file_output(false),
	   Delaunay(false), contour(false), shuffle(false), binary(false), xyz(false), 
           Section_file(false), number_of_points(-1), max_connected_comp(-1),
	   delta(.86), K_init(1.1), K_step(.1), K(5), out_format(0),
	   NB_BORDER_MAX(15), red(0), green(0), blue(0), no_header(false), area(0), perimeter(0)
  { 
    std::strcpy(finname,"finput");
    std::strcpy(foutname,"foutput"); 
  }

    char program[100];
    char finname[100];
    char foutname[100];
    bool file_input;
    bool file_output;
    bool Delaunay;
    bool contour;
    bool shuffle;
    bool binary;
    bool xyz;
    bool Section_file;
    int  number_of_points;
    int  max_connected_comp;
    double delta;
    double K_init;
    double K_step;
    double K;
    int out_format;
    int NB_BORDER_MAX;
    double red, green, blue;
    bool no_header;
  double area, perimeter;
};


} // namespace NUAGE

#endif  // NUAGE_OPTIONS_H
