/* Advanced Vala Sample Code */
using GLib;
using Gtk;

namespace GtkTextCompletion{

	public interface Provider {
		public abstract List get_data(TextView completion, String event_name);
		public abstract void data_selected(TextView completion, pointer data);
	}
}
