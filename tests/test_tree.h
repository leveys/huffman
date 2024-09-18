#ifndef TEST_TREE_H
#define TEST_TREE_H

#include "../src/huffman.h"

#define DEFAULT_STRING "Ooh ooooh\n\
oooooooh ooooooh\n\
ooooooooooh oooooooooh\n\
ooooooooooooh oooooooooooh\n\
ooooh ooooh oooh oooh oooh\n\
oooh oooh oooh ooh\n\
Oh, hush thee, my baby, the night is behind us,\n\
  And black are the waters that sparkled so greeeeeeeeeen.\n\
The moooooooooooooooooooooooooooooooooooon,\n\
  looks downward to find us\n\
  At rest in the hollows that rustle between.\n\
Where billow meets billow, there soft be thy pillow;\n\
  Ah, weary wee flipperling, curl at thy easeeeeeeee!\n\
The storm shall not wake theeeeeeeeeeee, nor shark overtake theeeeeeeeeeeee,\n\
  Asleep in the aaaaaaaaaaaaaaaaarms of the slow-swinging seas.\n\
  Asleep in the aaaaaaaaaaaaaaaaarms of the slow-swinging seas.\n\
Ooh ooooh oooooooh\n\
ooooooh ooooooooooooh\n\
oooooooooooh\n\
ooooh oooooooooooh\n\
ooooh oooooh ooooooh\n\
oooooooooooh\n\
ooooooh\n\
ooooooh\n\
oooooh ooooooh ooooooh oooooh\n\
oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooh\n"

void make_test_tree(Tree* tree, const char* str, int chars_per_node);
int tree_is_binary(Tree* tree, int verbose);
int tree_check_weights(Tree* tree, int verbose);
int tree_check_order(Tree* tree, int verbose);
int tree_check_num_nodes(Tree* tree, int verbose);
int tree_check_brothers(Tree* tree, int verbose);
int tree_check_size(Tree* tree, int verbose);

#endif // TEST_TREE_H