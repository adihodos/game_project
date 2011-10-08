#ifndef CONTROL_CONTAINER_H__
#define CONTROL_CONTAINER_H__

#include <algorithm>
#include <list>
#include <memory>
#include <stack>

struct KeyboardEventArgs;

namespace game_ui {

class Widget2;

class IScreenComponentContainer {
protected :
  typedef std::list<std::shared_ptr<Widget2>> ChildList;
  typedef std::list<std::shared_ptr<Widget2>>::value_type ChildListElementType;
  typedef std::list<std::shared_ptr<Widget2>>::iterator ChildListIterator;
  typedef std::list<std::shared_ptr<Widget2>>::const_iterator ChildListConstIterator;

public :
  IScreenComponentContainer();

  virtual ~IScreenComponentContainer();

  void add_child(Widget2* child);

  void remove_child(Widget2* child);

  ChildListIterator get_children_begin_iterator() {
    return children_.begin();
  }

  ChildListIterator get_children_end_interator() {
    return children_.end();
  }

protected :
  template<typename MemFunPtr, typename ArgType>
  Widget2* child_consumes_event(MemFunPtr mem_fun, ArgType fun_arg);

  ChildList children_;
};

} // nss game_ui

template<typename MemFunPtr, typename ArgType>
game_ui::Widget2*
game_ui::IScreenComponentContainer::child_consumes_event(
  MemFunPtr mem_fun, 
  ArgType fun_arg
  )
{
  auto child_itr = std::find_if(
    children_.rbegin(), children_.rend(),
    [mem_fun, fun_arg](ChildListElementType this_child) {
      return ((this_child.get())->*mem_fun)(fun_arg);
  });

  return child_itr == children_.rend() ? nullptr : child_itr->get();
}

#endif // CONTROL_CONTANER_H__