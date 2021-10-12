#include <tdl.h>

int
main (void)
{
  tdl_canvas_t *canv = tdl_canvas ();
  tdl_text_t text = tdl_default_text ("Hello world!");

  /* Hard clear of your terminal :P */
  system ("clear");

  tdl_set_cursor_pos (canv, tdl_point (6, 4));
  tdl_print(canv, text);

  /* Display all canvas changes to terminal */
  tdl_display (canv);

  /* Freeing memory from our objects */
  tdl_destroy_canvas (canv);
  tdl_text_free (text);

  return EXIT_SUCCESS;
}
