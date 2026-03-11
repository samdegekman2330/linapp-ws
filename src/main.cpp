#include <adwaita.h>

typedef struct {
    GtkWidget *chat_label;
    GtkWidget *entry;
    GtkWidget *window;
} AppWidgets;

static void on_send_clicked(GtkButton *btn, AppWidgets *widgets) {
    const char *text = gtk_editable_get_text(GTK_EDITABLE(widgets->entry));
    if (g_strcmp0(text, "") != 0) {
        char *markup = g_strdup_printf("<span size='large'>Je stuurde: %s</span>", text);
        gtk_label_set_markup(GTK_LABEL(widgets->chat_label), markup);
        g_free(markup);
        gtk_editable_set_text(GTK_EDITABLE(widgets->entry), "");
    }
}

static void show_about(GtkButton *btn, AppWidgets *widgets) {
    adw_show_about_dialog(GTK_WIDGET(widgets->window),
        "application-name", "LinApp-WS",
        "application-icon", "com.sama.linapp_ws",
        "developer-name", "Sama",
        "version", "0.1.0-alpha",
        "website", "https://github.com/samdegekman2330/linapp-ws",
        "issue-url", "https://github.com/samdegekman2330/linapp-ws/issues",
        "comments", "Omdat Meta te lui is voor een native Linux client.",
        NULL);
}

static void on_row_selected(GtkListBox *box, GtkListBoxRow *row, AppWidgets *widgets) {
    if (!row) return;
    GtkWidget *child = gtk_list_box_row_get_child(row);
    const char *name = "Onbekend";
    if (ADW_IS_ACTION_ROW(child)) {
        name = adw_preferences_row_get_title(ADW_PREFERENCES_ROW(child));
    }
    char *markup = g_strdup_printf("<span size='xx-large' weight='bold'>Chat met %s</span>", name);
    gtk_label_set_markup(GTK_LABEL(widgets->chat_label), markup);
    g_free(markup);
}

static void add_contact(GtkWidget *list_box, const char *name, const char *status) {
    GtkWidget *row = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row), name);
    adw_action_row_set_subtitle(ADW_ACTION_ROW(row), status);
    GtkWidget *icon = gtk_image_new_from_icon_name("avatar-default-symbolic");
    adw_action_row_add_prefix(ADW_ACTION_ROW(row), icon);
    gtk_list_box_append(GTK_LIST_BOX(list_box), row);
}

static void activate(AdwApplication *app) {
    AppWidgets *widgets = g_new(AppWidgets, 1);
    widgets->window = adw_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(widgets->window), "LinApp-WS");
    gtk_window_set_default_size(GTK_WINDOW(widgets->window), 900, 700);

    GtkWidget *toolbar_view = adw_toolbar_view_new();
    GtkWidget *header_bar = adw_header_bar_new();
    adw_toolbar_view_add_top_bar(ADW_TOOLBAR_VIEW(toolbar_view), header_bar);

    GtkWidget *btn_about = gtk_button_new_from_icon_name("help-about-symbolic");
    adw_header_bar_pack_end(ADW_HEADER_BAR(header_bar), btn_about);
    g_signal_connect(btn_about, "clicked", G_CALLBACK(show_about), widgets);

    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_paned_set_position(GTK_PANED(paned), 300);

    GtkWidget *scrolled = gtk_scrolled_window_new();
    GtkWidget *list_box = gtk_list_box_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), list_box);
    add_contact(list_box, "Elon Musk", "Richting Mars...");
    add_contact(list_box, "Mark Zuckerberg", "Meta is de toekomst.");
    add_contact(list_box, "Arch Linux Bot", "I use Arch btw.");

    GtkWidget *chat_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    widgets->chat_label = gtk_label_new("Selecteer een chat");
    gtk_widget_set_vexpand(widgets->chat_label, TRUE);
    gtk_box_append(GTK_BOX(chat_container), widgets->chat_label);

    GtkWidget *bottom_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_margin_start(bottom_bar, 12);
    gtk_widget_set_margin_end(bottom_bar, 12);
    gtk_widget_set_margin_top(bottom_bar, 12);
    gtk_widget_set_margin_bottom(bottom_bar, 12);
    
    widgets->entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->entry), "Typ een bericht...");
    gtk_widget_set_hexpand(widgets->entry, TRUE);
    
    GtkWidget *btn_send = gtk_button_new_from_icon_name("mail-send-symbolic");
    gtk_widget_add_css_class(btn_send, "suggested-action");
    g_signal_connect(btn_send, "clicked", G_CALLBACK(on_send_clicked), widgets);
    g_signal_connect(widgets->entry, "activate", G_CALLBACK(on_send_clicked), widgets);

    gtk_box_append(GTK_BOX(bottom_bar), widgets->entry);
    gtk_box_append(GTK_BOX(bottom_bar), btn_send);
    gtk_box_append(GTK_BOX(chat_container), bottom_bar);

    g_signal_connect(list_box, "row-selected", G_CALLBACK(on_row_selected), widgets);
    gtk_paned_set_start_child(GTK_PANED(paned), scrolled);
    gtk_paned_set_end_child(GTK_PANED(paned), chat_container);
    adw_toolbar_view_set_content(ADW_TOOLBAR_VIEW(toolbar_view), paned);
    adw_application_window_set_content(ADW_APPLICATION_WINDOW(widgets->window), toolbar_view);

    gtk_window_present(GTK_WINDOW(widgets->window));
}

int main(int argc, char **argv) {
    AdwApplication *app = adw_application_new("com.sama.linapp_ws", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    return g_application_run(G_APPLICATION(app), argc, argv);
}
