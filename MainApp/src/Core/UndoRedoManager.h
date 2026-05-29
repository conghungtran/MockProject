#pragma once
#include<stack>

namespace PrinterHub {
	namespace Core {
		class ICommand {

		};
		class UndoRedoManager {
		private:
			std::stack<ICommand*> undoStack;
			std::stack<ICommand*> redoStack;
		public:
			void AddCommand(ICommand* cmd);
			void undo();
			void redo();
		};
	}
}