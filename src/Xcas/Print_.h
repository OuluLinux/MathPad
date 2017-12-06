#if 0

#ifndef _Xcas_Print_h_
#define _Xcas_Print_h_

namespace xcas {

extern int printer_format;
extern bool printer_landscape;
extern double pixel_scale; // size of 1 pixel on a page for printing

void widget_ps_print(Fl_Widget * widget, const std::string & fname0, bool eps = false, int pngpdf = 7, bool preview = true, bool hidemouseparam = true, bool askusersize = true);
void widget_print(Fl_Widget * widget);

}

#endif
#endif
