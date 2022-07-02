/* This file is part of the the dci project. Copyright (C) 2013-2022 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "../utils/bundle.hpp"
#include "test/victimInterface.hpp"

using namespace dci::idl::stiac::test;

namespace
{
    struct Bundle : public ::utils::Bundle
    {
        Victim<>            _i1;
        Victim<>::Opposite  _i2;

        Future<void>    _v1;
        Promise<void>   _v2;

        Future<uint32>    _u1;
        Promise<uint32>   _u2;

        Bundle()
            : ::utils::Bundle()
        {
            _l2->got() += [&](idl::Interface&& i)
            {
                _i2 = i;

                _i2->in_withResult() += [&]
                {
                    return _v2.future();
                };
                _i2->in_withResult2() += [&]
                {
                    return _u2.future();
                };

                return readyFuture<void>();
            };


            _i1.init();
            _l1->put(idl::Interface(_i1.opposite())).value();

            _v1 = _i1->in_withResult();
            _u1 = _i1->in_withResult2();
        }
    };
}

/////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
TEST(module_stiac, localEdge_future)
{
    //for(int k(0); k<10000; ++k)
    {
        //бросить одну сторону
        {
            int cnt = 0;
            Bundle b;

            b._v1.then() += [&](auto f)
            {
                EXPECT_TRUE(f.resolvedCancel());
                cnt++;
            };
            b._v2 = Promise<void>();

            EXPECT_EQ(1, cnt);
        }

        {
            int cnt = 0;
            Bundle b;

            b._v2.canceled() += [&]
            {
                cnt++;
            };
            b._v1 = Future<void>();

            EXPECT_EQ(1, cnt);
        }

        //отмена
        {
            int cnt = 0;
            Bundle b;

            b._v1.then() += [&](auto f)
            {
                EXPECT_TRUE(f.resolvedCancel());
                cnt++;
            };
            b._v2.resolveCancel();

            EXPECT_EQ(1, cnt);
        }

        {
            int cnt = 0;
            Bundle b;

            b._v2.canceled() += [&]
            {
                cnt++;
            };
            b._v1.resolveCancel();

            EXPECT_EQ(1, cnt);
        }

        //ошибка
        {
            int cnt = 0;
            Bundle b;

            b._v1.then() += [&](auto f)
            {
                EXPECT_TRUE(f.resolvedException());
                try
                {
                    f.value();
                }
                catch(std::runtime_error& e)
                {
                    EXPECT_EQ(std::string("some exception"), e.what());
                    cnt++;
                }
            };
            b._v2.resolveException(std::make_exception_ptr(std::runtime_error("some exception")));

            EXPECT_EQ(1, cnt);
        }

        //значение
        {
            int cnt = 0;
            Bundle b;

            b._v1.then() += [&](auto f)
            {
                EXPECT_TRUE(f.resolvedValue());
                cnt++;
            };
            b._v2.resolveValue();

            EXPECT_EQ(1, cnt);
        }

        //значение
        {
            int cnt = 0;
            Bundle b;

            b._u1.then() += [&](auto f)
            {
                EXPECT_TRUE(f.resolvedValue());
                EXPECT_EQ(f.value(), 42u);
                cnt++;
            };
            b._u2.resolveValue(42u);

            EXPECT_EQ(1, cnt);
        }
    }
}
