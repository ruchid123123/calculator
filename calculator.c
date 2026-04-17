#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static GtkWidget *display;
static char current_input[256] = "0";
static char last_operator = 0;
static double first_operand = 0;
static int new_number = 1;

static void update_display() {
    gtk_entry_set_text(GTK_ENTRY(display), current_input);
}

static void on_number_clicked(GtkWidget *widget, gpointer data) {
    const char *digit = (const char *)data;
    if (new_number) {
        strcpy(current_input, digit);
        new_number = 0;
    } else {
        if (strlen(current_input) < 250) {
            strcat(current_input, digit);
        }
    }
    update_display();
}

static void on_operator_clicked(GtkWidget *widget, gpointer data) {
    const char *op = (const char *)data;
    first_operand = atof(current_input);
    last_operator = op[0];
    new_number = 1;
}

static void on_equal_clicked(GtkWidget *widget, gpointer data) {
    double second_operand = atof(current_input);
    double result = 0;

    switch (last_operator) {
        case '+': result = first_operand + second_operand; break;
        case '-': result = first_operand - second_operand; break;
        case '*': result = first_operand * second_operand; break;
        case '/':
            if (second_operand != 0) {
                result = first_operand / second_operand;
            } else {
                strcpy(current_input, "Error");
                update_display();
                new_number = 1;
                last_operator = 0;
                return;
            }
            break;
        default:
            return;
    }

    snprintf(current_input, sizeof(current_input), "%.10g", result);
    update_display();
    new_number = 1;
    last_operator = 0;
}

static void on_clear_clicked(GtkWidget *widget, gpointer data) {
    strcpy(current_input, "0");
    first_operand = 0;
    last_operator = 0;
    new_number = 1;
    update_display();
}

static void on_delete_clicked(GtkWidget *widget, gpointer data) {
    int len = strlen(current_input);
    if (len > 1) {
        current_input[len - 1] = '\0';
    } else {
        strcpy(current_input, "0");
        new_number = 1;
    }
    update_display();
}

static void on_dot_clicked(GtkWidget *widget, gpointer data) {
    if (new_number) {
        strcpy(current_input, "0.");
        new_number = 0;
    } else if (strchr(current_input, '.') == NULL) {
        strcat(current_input, ".");
    }
    update_display();
}

static GtkWidget *create_button(const char *label, GCallback callback, gpointer data) {
    GtkWidget *btn = gtk_button_new_with_label(label);
    g_signal_connect(btn, "clicked", callback, data);
    return btn;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "计算器");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 400);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    display = gtk_entry_new();
    gtk_entry_set_alignment(GTK_ENTRY(display), 1);
    gtk_editable_set_editable(GTK_EDITABLE(display), FALSE);
    gtk_entry_set_text(GTK_ENTRY(display), "0");
    gtk_widget_set_size_request(display, -1, 50);
    gtk_box_pack_start(GTK_BOX(vbox), display, FALSE, FALSE, 0);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);

    static char labels[4][4][4] = {
        {"C",   "/",   "*",   "-"},
        {"7",   "8",   "9",   "+"},
        {"4",   "5",   "6",   "DEL"},
        {"1",   "2",   "3",   "="}
    };

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            const char *lbl = labels[row][col];

            GtkWidget *btn;
            static char *ops = "+-*/";

            if (strcmp(lbl, "C") == 0) {
                btn = create_button(lbl, G_CALLBACK(on_clear_clicked), NULL);
            } else if (strcmp(lbl, "=") == 0) {
                btn = create_button(lbl, G_CALLBACK(on_equal_clicked), NULL);
                gtk_grid_attach(GTK_GRID(grid), btn, col, row, 1, 2);
                continue;
            } else if (strcmp(lbl, "DEL") == 0) {
                btn = create_button(lbl, G_CALLBACK(on_delete_clicked), NULL);
            } else if (strchr(ops, lbl[0])) {
                static const char *op_ptrs[] = {"+", "-", "*", "/"};
                const char *op = NULL;
                for (int i = 0; i < 4; i++) if (strcmp(lbl, op_ptrs[i]) == 0) { op = op_ptrs[i]; break; }
                btn = create_button(lbl, G_CALLBACK(on_operator_clicked), (gpointer)op);
            } else if (strcmp(lbl, ".") == 0) {
                btn = create_button(lbl, G_CALLBACK(on_dot_clicked), NULL);
            } else {
                btn = create_button(lbl, G_CALLBACK(on_number_clicked), (gpointer)lbl);
            }

            gtk_grid_attach(GTK_GRID(grid), btn, col, row, 1, 1);
        }
    }

    // 0 spans 2 columns
    GtkWidget *btn0 = create_button("0", G_CALLBACK(on_number_clicked), (gpointer)"0");
    gtk_grid_attach(GTK_GRID(grid), btn0, 0, 4, 2, 1);

    GtkWidget *btnDot = create_button(".", G_CALLBACK(on_dot_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), btnDot, 2, 4, 1, 1);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
