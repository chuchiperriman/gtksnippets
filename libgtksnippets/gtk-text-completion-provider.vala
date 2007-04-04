/* Advanced Vala Sample Code */
using GLib;

namespace GtkTextCompletion{

	public interface Provider {
		public abstract List get_data(Object completion, String event_name);
		public abstract void data_selected(Object completion, Object data);
	}
}
