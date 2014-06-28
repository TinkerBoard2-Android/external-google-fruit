/*
 * Copyright 2014 Google Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FRUIT_INJECTOR_TEMPLATES_H
#define FRUIT_INJECTOR_TEMPLATES_H

#include "../component.h"

namespace fruit {

template <typename... P>
Injector<P...>::Injector(fruit::impl::ComponentStorage& storage)
  : storage(&storage, fruit::impl::NopDeleter<fruit::impl::ComponentStorage>()) {
}

template <typename... P>
Injector<P...>::Injector(const Comp& component)
  : storage(std::make_shared<fruit::impl::ComponentStorage>(component.storage)) {
};

template <typename... P>
Injector<P...>::Injector(Comp&& component)
  : storage(std::make_shared<fruit::impl::ComponentStorage>(std::move(component.storage))) {
};

template <typename... P>
template <typename ParentInjector, typename ChildComp>
Injector<P...>::Injector(const ParentInjector& parentInjector, const ChildComp& component)
  : storage(std::make_shared<fruit::impl::ComponentStorage>(component.storage)) {
  using ParentComp = typename ParentInjector::Comp;
  using Comp1 = decltype(
    fruit::createComponent()
      .install(std::declval<ParentComp>())
      .install(std::declval<ChildComp>()));
  FruitDelegateCheck(fruit::impl::CheckComponentEntails<Comp1, Comp>);
  storage->setParent(parentInjector.storage.get());
}

template <typename... P>
template <typename ParentInjector, typename ChildComp>
Injector<P...>::Injector(const ParentInjector& parentInjector, ChildComp&& component)
  : storage(std::make_shared<fruit::impl::ComponentStorage>(std::move(component.storage))) {
  using ParentComp = typename ParentInjector::Comp;
  using Comp1 = decltype(
    fruit::createComponent()
      .install(std::declval<ParentComp>())
      .install(std::declval<ChildComp>()));
  FruitDelegateCheck(fruit::impl::CheckComponentEntails<Comp1, Comp>);
  storage->setParent(parentInjector.storage.get());
}

template <typename... P>
template <typename T>
T Injector<P...>::get() {
  static_assert(fruit::impl::is_in_list<impl::GetClassForType<T>, Ps>::value,
                "trying to get an instance of T, but it is not provided by this injector");
  return storage->template get<T>();
}

template <typename... P>
template <typename T>
Injector<P...>::operator T() {
  return get<T>();
}


} // namespace fruit


#endif // FRUIT_INJECTOR_TEMPLATES_H
