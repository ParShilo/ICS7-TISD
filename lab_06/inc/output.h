#ifndef OUTPUT_H__
#define OUTPUT_H__

void generate_dot_highlight(node_t *root, FILE *file, int highlight_duplicates);
int create_dot_file(node_t *root, const char *filename, int highlight_duplicates);
void show_image(const char *image_path);

#endif