#include <stdbool.h>
#include <stdio.h>
#include <tdl.h>

int
main (void)
{
  tdl_canvas_t *canv = tdl_canvas ();
  tdl_text_t text = tdl_default_text ("Hello world!");

  /* Disable cursor and enable alternate screen */
  tdl_terminal_set_alternate_screen (true);
  tdl_terminal_set_cursor (false);

  /* Set the cursor to the position to print text in the middle of the screen
   */
  tdl_set_cursor_pos (
      canv, tdl_point ((int)(canv->size.width - text.string->length) / 2,
                       (int)canv->size.height / 2));
  tdl_print(canv, text);

  /* Display all canvas changes to terminal */
  tdl_display (canv);

  getchar ();
  
  tdl_terminal_set_cursor (true);
  tdl_terminal_set_alternate_screen (false);

  /* Freeing memory from our objects */
  tdl_destroy_canvas (canv);
  tdl_text_free (text);

  return EXIT_SUCCESS;
}
