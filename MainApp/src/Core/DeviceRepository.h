#pragma once
#include<string>
#include<vector>
#include<algorithm>

namespace PrinterHub {
	namespace Core {
		template <typename T>
		class DeviceRepository {
		private:
			std::vector<T> items;
		public:
			void Add(const T& item) {
				items.push_back(item);
			}

			bool Remove(const std::string& id) {
				auto it = std::find_if(items.begin(), items.end(), [&id](const T& item) {return item.getId() == id;});
					if (it != items.end()) {
						items.erase(it);
						return true;
				}
				return false;
			}

			T* findById(const std::string& id){
				for (T& item : items) {
					if (item.getId() == id){
						return &item;
					}
				}
				return nullptr;
			}

			const std::vector<T>& getAll() const {
				return items;
			}

			size_t Count() const {
				return items.size();
			}
		};
	}
}