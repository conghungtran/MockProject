#include"pch.h"
#include"UndoRedoManager.h"

namespace PrinterHub {
	namespace Core {
		void UndoRedoManager::undo() {
			if (!undoStack.empty()) {
				ICommand* cmd = undoStack.top();
				undoStack.pop();
				redoStack.push(cmd);
			}
		}

		void UndoRedoManager::redo() {
			if (!redoStack.empty()) {
				ICommand* cmd = redoStack.top();
				redoStack.pop();
				undoStack.push(cmd);
			}
		}

		void UndoRedoManager::AddCommand(ICommand* cmd) {
			undoStack.push(cmd);
			while (!redoStack.empty()) {
				redoStack.pop();
			}
		}
	}
}