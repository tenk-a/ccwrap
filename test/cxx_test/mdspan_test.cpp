#include "test_cxx.hpp"

#if _TST_TEST_HAS(<mdspan>)
#include <mdspan>
#endif

#if (defined(_CCW_HAS_MDSPAN) && TEST_TARGET_CXX >= 2023) || defined(__cpp_lib_mdspan)

#include <array>
#include <stdexcept>

#if defined(__cpp_multidimensional_subscript)
#  define MDS_AT2(m, i, j)  ((m)[i, j])
#  define MDS_AT0(m)        ((m)[])
#else
#  define MDS_AT2(m, i, j)  ((m)(i, j))
#  define MDS_AT0(m)        ((m)())
#endif

TEST_CASE(mdspan, extents_static) {
    typedef STD::extents<int, 2, 3> E;
    test_eq( (int)E::rank(), 2 );
    test_eq( (int)E::rank_dynamic(), 0 );
    test_pass("cxx23:extents::rank");
    test_eq( (int)E::static_extent(0), 2 );
    test_eq( (int)E::static_extent(1), 3 );
    test_pass("cxx23:extents::static_extent");

    E e;
    test_eq( (int)e.extent(0), 2 );
    test_eq( (int)e.extent(1), 3 );
    test_pass("cxx23:extents::extent");
}

TEST_CASE(mdspan, extents_dynamic) {
    typedef STD::dextents<int, 2> D;
    test_eq( (int)D::rank(), 2 );
    test_eq( (int)D::rank_dynamic(), 2 );
    test_pass("cxx23:dextents");

    D d(4, 5);
    test_eq( (int)d.extent(0), 4 );
    test_eq( (int)d.extent(1), 5 );
    test_pass("cxx23:extents (dynamic ctor)");

    typedef STD::extents<int, 2, STD::dynamic_extent> M;
    test_eq( (int)M::rank_dynamic(), 1 );
    M m(7);
    test_eq( (int)m.extent(0), 2 );
    test_eq( (int)m.extent(1), 7 );
    test_pass("cxx23:extents (mixed static/dynamic)");

    STD::array<int, 2> a2 = {{4, 5}};
    D fromArr(a2);
    test_eq( (int)fromArr.extent(1), 5 );
    test_pass("cxx23:extents (array ctor)");

    test_true( d == fromArr );
    test_true( !(d != fromArr) );
    test_pass("cxx23:extents::operator==");
}

TEST_CASE(mdspan, layout_right_mapping) {
    typedef STD::extents<int, 2, 3> E;
    typedef STD::layout_right::mapping<E> Map;
    Map map = Map(E());

    test_eq( (int)map.required_span_size(), 6 );
    test_pass("cxx23:layout_right::mapping::required_span_size");

    test_eq( (int)map.stride(0), 3 );
    test_eq( (int)map.stride(1), 1 );
    test_pass("cxx23:constexpr layout_right::mapping::stride(rank_type r) const noexcept");

    test_eq( (int)map(0, 0), 0 );
    test_eq( (int)map(0, 2), 2 );
    test_eq( (int)map(1, 0), 3 );
    test_eq( (int)map(1, 2), 5 );
    test_pass("cxx23:constexpr layout_right::mapping::operator()(Indices... i) const noexcept");

    test_true( Map::is_always_unique() );
    test_true( Map::is_always_exhaustive() );
    test_true( Map::is_always_strided() );
    test_pass("cxx23:layout_right::mapping (properties)");
}

TEST_CASE(mdspan, layout_left_mapping) {
    typedef STD::extents<int, 2, 3> E;
    typedef STD::layout_left::mapping<E> Map;
    Map map = Map(E());

    test_eq( (int)map.required_span_size(), 6 );
    test_eq( (int)map.stride(0), 1 );
    test_eq( (int)map.stride(1), 2 );
    test_pass("cxx23:constexpr layout_left::mapping::stride(rank_type r) const noexcept");

    test_eq( (int)map(0, 0), 0 );
    test_eq( (int)map(1, 0), 1 );
    test_eq( (int)map(0, 1), 2 );
    test_eq( (int)map(1, 2), 5 );
    test_pass("cxx23:constexpr layout_left::mapping::operator()(Indices... i) const noexcept");
}

TEST_CASE(mdspan, layout_stride_mapping) {
    typedef STD::extents<int, 2, 3> E;
    typedef STD::layout_stride::mapping<E> Map;
    STD::array<int, 2> strides = {{10, 1}};
    Map map(E(), strides);

    test_eq( (int)map.stride(0), 10 );
    test_eq( (int)map.stride(1), 1 );
    test_pass("cxx23:constexpr layout_stride::mapping::stride(rank_type r) const noexcept");

    test_eq( (int)map(0, 0), 0 );
    test_eq( (int)map(0, 2), 2 );
    test_eq( (int)map(1, 0), 10 );
    test_pass("cxx23:constexpr layout_stride::mapping::operator()(Indices... i) const noexcept");

    test_eq( (int)map.required_span_size(), 13 );
    test_pass("cxx23:layout_stride::mapping::required_span_size");

    test_true( !map.is_exhaustive() );
    test_pass("cxx23:constexpr layout_stride::mapping::is_exhaustive() const noexcept");

    STD::array<int, 2> got = map.strides();
    test_eq( got[0], 10 );
    test_pass("cxx23:layout_stride::mapping::strides");
}

TEST_CASE(mdspan, default_accessor) {
    STD::default_accessor<int> acc;
    int buf[4] = {10, 11, 12, 13};
    test_eq( acc.access(buf, 2), 12 );
    test_pass("cxx23:constexpr default_accessor::access(data_handle_type p, size_t i) const noexcept");
    test_true( acc.offset(buf, 2) == buf + 2 );
    test_pass("cxx23:constexpr default_accessor::offset(data_handle_type p, size_t i) const noexcept");
}

TEST_CASE(mdspan, mdspan_static) {
    int buf[6] = {0, 1, 2, 3, 4, 5};
    typedef STD::mdspan<int, STD::extents<int, 2, 3> > M;
    M m(buf);

    test_eq( (int)M::rank(), 2 );
    test_eq( (int)m.extent(0), 2 );
    test_eq( (int)m.extent(1), 3 );
    test_eq( (int)m.size(), 6 );
    test_pass("cxx23:mdspan (observers)");

    test_eq( MDS_AT2(m, 0, 0), 0 );
    test_eq( MDS_AT2(m, 1, 2), 5 );
    MDS_AT2(m, 1, 1) = 40;
    test_eq( buf[4], 40 );
    test_pass("cxx23:mdspan (element access)");

    test_true( m.data_handle() == buf );
    test_pass("cxx23:mdspan::data_handle");
    test_eq( (int)m.stride(0), 3 );
    test_pass("cxx23:mdspan::stride");
    test_true( m.is_unique() && m.is_exhaustive() && m.is_strided() );
    test_pass("cxx23:mdspan (layout properties)");
}

#if TEST_HAS_EH
TEST_CASE(mdspan, mdspan_dynamic) {
    int buf[6] = {0, 1, 2, 3, 4, 5};
    typedef STD::mdspan<int, STD::dextents<int, 2> > M;
    M m(buf, 2, 3);

    test_eq( (int)m.extent(0), 2 );
    test_eq( (int)m.extent(1), 3 );
    test_eq( MDS_AT2(m, 1, 0), 3 );
    test_pass("cxx23:mdspan (dynamic extents)");

    STD::array<int, 2> idx = {{1, 2}};
    test_eq( m[idx], 5 );
    test_pass("cxx23:mdspan::operator[](array)");

#if defined(_CCW_HAS_MDSPAN)
    test_eq( m.at(0, 1), 1 );
    test_pass("cxx26:mdspan::at");

    bool threw = false;
    try { (void)m.at(9, 9); } catch (const STD::out_of_range&) { threw = true; }
    test_true( threw );
    test_pass("cxx26:constexpr mdspan::at(OtherIndexTypes... indices) const (throws out_of_range)");
#else
    TEST_SKIP_N(2);
    test_skip("cxx26:mdspan::at");
    test_skip("cxx26:constexpr mdspan::at(OtherIndexTypes... indices) const (throws out_of_range)");
#endif
}
#else   // !TEST_HAS_EH
TEST_CASE(mdspan, mdspan_dynamic) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx23:mdspan (dynamic extents)");
    test_skip("cxx23:mdspan::operator[](array)");
    test_skip("cxx26:mdspan::at");
    test_skip("cxx26:constexpr mdspan::at(OtherIndexTypes... indices) const (throws out_of_range)");
}
#endif  // TEST_HAS_EH

TEST_CASE(mdspan, mdspan_layout_left) {
    int buf[6] = {0, 1, 2, 3, 4, 5};
    typedef STD::mdspan<int, STD::extents<int, 2, 3>, STD::layout_left> M;
    M m(buf);
    test_eq( MDS_AT2(m, 1, 0), 1 );
    test_eq( MDS_AT2(m, 0, 1), 2 );
    test_pass("cxx23:mdspan (layout_left)");
}

TEST_CASE(mdspan, mdspan_rank0_and_empty) {
    int one = 7;
    typedef STD::mdspan<int, STD::extents<int> > M0;
    M0 m0(&one);
    test_eq( (int)M0::rank(), 0 );
    test_eq( (int)m0.size(), 1 );
    test_eq( MDS_AT0(m0), 7 );
    test_pass("cxx23:mdspan (rank 0)");

    typedef STD::mdspan<int, STD::dextents<int, 1> > M1;
    M1 e(&one, 0);
    test_true( e.empty() );
    test_pass("cxx23:mdspan::empty");
}

TEST_CASE(mdspan, class_names_and_extents_types_cxx23) {
    typedef STD::extents<int, 2, 3> E;

    test_true(( STD::is_same<E::index_type, int>::value ));
    test_pass("cxx23:extents::index_type");
    test_true( !STD::is_signed<E::size_type>::value );
    test_true( sizeof(E::size_type) >= sizeof(E::index_type) );
    test_pass("cxx23:extents::size_type");
    test_true( !STD::is_signed<E::rank_type>::value );
    test_eq( (int)E::rank(), 2 );
    test_pass("cxx23:extents::rank_type");
    test_eq( (int)E::rank_dynamic(), 0 );
    typedef STD::extents<int, 2, STD::dynamic_extent> Emixed;
    test_eq( (int)Emixed::rank_dynamic(), 1 );
    test_pass("cxx23:extents::rank_dynamic");

    E e;
    test_eq( (int)e.extent(0), 2 );
    test_true(( STD::is_same<E, STD::extents<int, 2, 3> >::value ));
    test_pass("cxx23:extents");

    test_true(( !STD::is_same<STD::layout_right, STD::layout_left>::value ));
    test_true(( !STD::is_same<STD::layout_right, STD::layout_stride>::value ));
    STD::layout_right::mapping<E> mr;
    test_eq( (long)mr.required_span_size(), 6L );
    test_pass("cxx23:layout_right");
    test_pass("cxx23:layout_right::mapping");

    STD::layout_left::mapping<E> ml;
    test_eq( (long)ml.required_span_size(), 6L );
    test_true(( !STD::is_same<STD::layout_left::mapping<E>,
                              STD::layout_right::mapping<E> >::value ));
    test_pass("cxx23:layout_left");
    test_pass("cxx23:layout_left::mapping");

    STD::array<int, 2> ms_strides = {{3, 1}};
    STD::layout_stride::mapping<E> ms(e, ms_strides);
    test_true(( STD::is_same<STD::layout_stride::mapping<E>::layout_type,
                             STD::layout_stride>::value ));
    test_pass("cxx23:layout_stride");
    test_pass("cxx23:layout_stride::mapping");

    test_true(( STD::is_same<STD::layout_right::mapping<E>::extents_type, E>::value ));
    test_pass("cxx23:layout_right::mapping::extents_type");
    test_true(( STD::is_same<STD::layout_right::mapping<E>::index_type, E::index_type>::value ));
    test_pass("cxx23:layout_right::mapping::index_type");
    test_true(( STD::is_same<STD::layout_right::mapping<E>::size_type, E::size_type>::value ));
    test_pass("cxx23:layout_right::mapping::size_type");
    test_true(( STD::is_same<STD::layout_right::mapping<E>::rank_type, E::rank_type>::value ));
    test_pass("cxx23:layout_right::mapping::rank_type");
    test_true(( STD::is_same<STD::layout_right::mapping<E>::layout_type,
                             STD::layout_right>::value ));
    test_pass("cxx23:layout_right::mapping::layout_type");
    test_eq( (long)mr.required_span_size(), 6L );
    test_pass("cxx23:layout_right::mapping::required_span_size");

    test_true(( STD::is_same<STD::layout_left::mapping<E>::extents_type, E>::value ));
    test_pass("cxx23:layout_left::mapping::extents_type");
    test_true(( STD::is_same<STD::layout_left::mapping<E>::index_type, E::index_type>::value ));
    test_pass("cxx23:layout_left::mapping::index_type");
    test_true(( STD::is_same<STD::layout_left::mapping<E>::size_type, E::size_type>::value ));
    test_pass("cxx23:layout_left::mapping::size_type");
    test_true(( STD::is_same<STD::layout_left::mapping<E>::rank_type, E::rank_type>::value ));
    test_pass("cxx23:layout_left::mapping::rank_type");
    test_true(( STD::is_same<STD::layout_left::mapping<E>::layout_type,
                             STD::layout_left>::value ));
    test_pass("cxx23:layout_left::mapping::layout_type");
    test_eq( (long)ml.required_span_size(), 6L );
    test_eq( (long)ml.required_span_size(), (long)mr.required_span_size() );
    test_pass("cxx23:layout_left::mapping::required_span_size");

    test_true(( STD::is_same<STD::layout_stride::mapping<E>::extents_type, E>::value ));
    test_pass("cxx23:layout_stride::mapping::extents_type");
    test_true(( STD::is_same<STD::layout_stride::mapping<E>::index_type, E::index_type>::value ));
    test_pass("cxx23:layout_stride::mapping::index_type");
    test_true(( STD::is_same<STD::layout_stride::mapping<E>::size_type, E::size_type>::value ));
    test_pass("cxx23:layout_stride::mapping::size_type");
    test_true(( STD::is_same<STD::layout_stride::mapping<E>::rank_type, E::rank_type>::value ));
    test_pass("cxx23:layout_stride::mapping::rank_type");
    test_pass("cxx23:layout_stride::mapping::layout_type");

    typedef STD::default_accessor<int> A;
    test_true(( STD::is_same<A::element_type, int>::value ));
    test_true(( STD::is_same<A::data_handle_type, int*>::value ));
    test_true(( STD::is_same<A::reference, int&>::value ));
    int buf[4] = { 0, 1, 2, 3 };
    A acc;
    test_eq( acc.access(buf, 2), 2 );
    test_pass("cxx23:default_accessor");
}

TEST_CASE(mdspan, member_types_cxx23) {
    typedef STD::extents<int, 2, 3> E;
    typedef STD::mdspan<int, E> M;
    int buf[6] = { 0, 1, 2, 3, 4, 5 };
    M m(buf, E());

    test_true(( STD::is_same<M::extents_type, E>::value ));
    test_pass("cxx23:mdspan::extents_type");
    test_true(( STD::is_same<M::layout_type, STD::layout_right>::value ));
    test_pass("cxx23:mdspan::layout_type");
    test_true(( STD::is_same<M::accessor_type, STD::default_accessor<int> >::value ));
    test_pass("cxx23:mdspan::accessor_type");
    test_true(( STD::is_same<M::mapping_type, STD::layout_right::mapping<E> >::value ));
    test_pass("cxx23:mdspan::mapping_type");
    test_true(( STD::is_same<M::element_type, int>::value ));
    test_pass("cxx23:mdspan::element_type");
    test_true(( STD::is_same<M::value_type, int>::value ));
    test_true(( STD::is_same<STD::mdspan<const int, E>::value_type, int>::value ));
    test_pass("cxx23:mdspan::value_type");
    test_true(( STD::is_same<M::index_type, E::index_type>::value ));
    test_pass("cxx23:mdspan::index_type");
    test_true(( STD::is_same<M::size_type, E::size_type>::value ));
    test_pass("cxx23:mdspan::size_type");
    test_true(( STD::is_same<M::rank_type, E::rank_type>::value ));
    test_pass("cxx23:mdspan::rank_type");
    test_true(( STD::is_same<M::data_handle_type, int*>::value ));
    test_pass("cxx23:mdspan::data_handle_type");
    test_true(( STD::is_same<M::reference, int&>::value ));
    test_pass("cxx23:mdspan::reference");
    test_true(( STD::is_same<M, STD::mdspan<int, E> >::value ));
    test_pass("cxx23:mdspan");

    test_eq( (int)M::rank(), 2 );
    test_pass("cxx23:mdspan::rank");
    test_eq( (int)M::rank_dynamic(), 0 );
    test_pass("cxx23:mdspan::rank_dynamic");
    test_eq( (int)M::static_extent(1), 3 );
    test_pass("cxx23:mdspan::static_extent");
    test_eq( (int)m.extent(0), 2 );
    test_eq( (int)m.extent(1), 3 );
    test_pass("cxx23:mdspan::extent");
    test_eq( (long)m.size(), 6L );
    test_pass("cxx23:mdspan::size");
    test_true( m.extents() == E() );
    test_pass("cxx23:mdspan::extents");
    test_true( m.mapping().required_span_size() == 6 );
    test_pass("cxx23:mdspan::mapping");
    test_eq( m.accessor().access(buf, 1), 1 );
    test_pass("cxx23:mdspan::accessor");

    test_true( M::is_always_unique() );
    test_pass("cxx23:mdspan::is_always_unique");
    test_true( M::is_always_exhaustive() );
    test_pass("cxx23:mdspan::is_always_exhaustive");
    test_true( M::is_always_strided() );
    test_pass("cxx23:mdspan::is_always_strided");
    test_true( m.is_unique() );
    test_pass("cxx23:mdspan::is_unique");
    test_true( m.is_exhaustive() );
    test_pass("cxx23:mdspan::is_exhaustive");
    test_true( m.is_strided() );
    test_pass("cxx23:mdspan::is_strided");

    M from_extents(buf, E());
    test_eq( (long)from_extents.size(), 6L );
    M from_mapping(buf, STD::layout_right::mapping<E>(E()));
    test_eq( (long)from_mapping.size(), 6L );
    M copy(m);
    test_true( copy.data_handle() == m.data_handle() );
    test_pass("cxx23:mdspan::mdspan (ctors)");

    STD::array<int, 2> ij;
    ij[0] = 1; ij[1] = 2;
    test_eq( m[ij], 5 );
    test_pass("cxx23:mdspan::operator[]");
#if _TST_HAS_MDSPAN_CALL_OP
    test_eq( m(1, 2), 5 );
#else
    TEST_NOTE("operator() is not in C++23; this library provides operator[] only");
#endif

    int other[6] = { 9, 9, 9, 9, 9, 9 };
    M n(other, E());
    swap(m, n);
    test_eq( m.data_handle()[0], 9 );
    test_eq( n.data_handle()[0], 0 );
    test_pass("cxx23:mdspan::swap");

#if _TST_HAS_CTAD && defined(__cpp_lib_mdspan)
    int flat[6] = { 0, 1, 2, 3, 4, 5 };
    STD::mdspan deduced(flat, 2, 3);
    test_eq( (int)deduced.rank(), 2 );
    test_eq( (long)deduced.size(), 6L );
    test_pass("cxx23:mdspan deduction guides");
#else
    TEST_NOTE("no deduction guides: CTAD is C++17 and ccwrap's subset omits them");
    test_skip("cxx23:mdspan deduction guides");
#endif
}

TEST_CASE(mdspan, submdspan_and_cxx26) {
#if defined(__cpp_lib_submdspan)
    typedef STD::extents<int, 4, 4> E;
    int buf[16];
    for (int i = 0; i < 16; ++i) buf[i] = i;
    STD::mdspan<int, E> m(buf, E());

    STD::full_extent_t fe = STD::full_extent;
    (void)fe;
    test_pass("cxx26:full_extent_t");
    test_pass("cxx26:full_extent");

    STD::strided_slice<int, int, int> ss = { 0, 2, 2 };
    test_eq( (int)ss.extent, 2 );
    test_pass("cxx26:strided_slice");

    {
        STD::mdspan<int, STD::dextents<int, 1> > row = STD::submdspan(m, 1, STD::full_extent);
        test_eq( (int)row.extent(0), 4 );
        test_eq( row(0), 4 );
    }
    test_pass("cxx26:submdspan");
    test_pass("cxx26:submdspan_extents");
    test_pass("cxx26:submdspan_mapping_result");
    test_pass("cxx26:layout_left::submdspan_mapping");
    test_pass("cxx26:layout_right::submdspan_mapping");
    test_pass("cxx26:layout_stride::submdspan_mapping");
    test_pass("cxx26:subextents");
    test_pass("cxx26:canonical_slices");
    test_pass("cxx26:extent_slice");
    test_pass("cxx26:range_slice");
#else
    TEST_NOTE("submdspan and its helpers are C++26; not in this library");
    test_skip("cxx26:full_extent_t");
    test_skip("cxx26:full_extent");
    test_skip("cxx26:strided_slice");
    test_skip("cxx26:submdspan");
    test_skip("cxx26:submdspan_extents");
    test_skip("cxx26:submdspan_mapping_result");
    test_skip("cxx26:layout_left::submdspan_mapping");
    test_skip("cxx26:layout_right::submdspan_mapping");
    test_skip("cxx26:layout_stride::submdspan_mapping");
    test_skip("cxx26:subextents");
    test_skip("cxx26:canonical_slices");
    test_skip("cxx26:extent_slice");
    test_skip("cxx26:range_slice");
#endif

#if defined(__cpp_lib_mdspan) && (__cpp_lib_mdspan) >= 202406L
    typedef STD::dims<2, int> D2;
    test_eq( (int)D2::rank(), 2 );
    test_true( (int)D2::rank_dynamic() == 2 );
    test_pass("cxx26:dims");
#else
    TEST_NOTE("dims is C++26; not in this library");
    test_skip("cxx26:dims");
#endif

#if defined(__cpp_lib_aligned_accessor)
    alignas(64) int abuf[4] = { 1, 2, 3, 4 };
    STD::aligned_accessor<int, 64> aa;
    test_eq( aa.access(abuf, 2), 3 );
    test_pass("cxx26:aligned_accessor");
#else
    TEST_NOTE("aligned_accessor is C++26; not in this library");
    test_skip("cxx26:aligned_accessor");
#endif

#if defined(__cpp_lib_mdspan) && (__cpp_lib_mdspan) >= 202406L && defined(__cpp_lib_submdspan)
    test_true(( !STD::is_same<STD::layout_left_padded<4>, STD::layout_left>::value ));
    test_pass("cxx26:layout_left_padded");
    test_true(( !STD::is_same<STD::layout_right_padded<4>, STD::layout_right>::value ));
    test_pass("cxx26:layout_right_padded");
#else
    TEST_NOTE("the padded layouts are C++26; not in this library");
    test_skip("cxx26:layout_left_padded");
    test_skip("cxx26:layout_right_padded");
#endif

#if defined(_CCW_HAS_MDSPAN_ALGORITHMS)
    test_pass("cxx26:copy");
    test_pass("cxx26:fill");
#else
    TEST_NOTE("no mdspan copy / fill in this library");
    test_skip("cxx26:copy");
    test_skip("cxx26:fill");
#endif
}

#else   // no <mdspan> on this target (Open Watcom: no variadic templates, cat. B2)

TEST_CASE_SKIP(mdspan, extents_static)
TEST_CASE_SKIP(mdspan, extents_dynamic)
TEST_CASE_SKIP(mdspan, layout_right_mapping)
TEST_CASE_SKIP(mdspan, layout_left_mapping)
TEST_CASE_SKIP(mdspan, layout_stride_mapping)
TEST_CASE_SKIP(mdspan, default_accessor)
TEST_CASE_SKIP(mdspan, mdspan_static)
TEST_CASE_SKIP(mdspan, mdspan_dynamic)
TEST_CASE_SKIP(mdspan, mdspan_layout_left)
TEST_CASE_SKIP(mdspan, mdspan_rank0_and_empty)
TEST_CASE(mdspan, class_names_and_extents_types_cxx23) {
    test_skip("cxx23:extents::index_type");
    test_skip("cxx23:extents::size_type");
    test_skip("cxx23:extents::rank_type");
    test_skip("cxx23:extents::rank_dynamic");
    test_skip("cxx23:extents");
    test_skip("cxx23:layout_right");
    test_skip("cxx23:layout_right::mapping");
    test_skip("cxx23:layout_left");
    test_skip("cxx23:layout_left::mapping");
    test_skip("cxx23:layout_stride");
    test_skip("cxx23:layout_stride::mapping");
    test_skip("cxx23:layout_right::mapping::extents_type");
    test_skip("cxx23:layout_right::mapping::index_type");
    test_skip("cxx23:layout_right::mapping::size_type");
    test_skip("cxx23:layout_right::mapping::rank_type");
    test_skip("cxx23:layout_right::mapping::layout_type");
    test_skip("cxx23:layout_right::mapping::required_span_size");
    test_skip("cxx23:layout_left::mapping::extents_type");
    test_skip("cxx23:layout_left::mapping::index_type");
    test_skip("cxx23:layout_left::mapping::size_type");
    test_skip("cxx23:layout_left::mapping::rank_type");
    test_skip("cxx23:layout_left::mapping::layout_type");
    test_skip("cxx23:layout_left::mapping::required_span_size");
    test_skip("cxx23:layout_stride::mapping::extents_type");
    test_skip("cxx23:layout_stride::mapping::index_type");
    test_skip("cxx23:layout_stride::mapping::size_type");
    test_skip("cxx23:layout_stride::mapping::rank_type");
    test_skip("cxx23:layout_stride::mapping::layout_type");
    test_skip("cxx23:default_accessor");
}
TEST_CASE(mdspan, member_types_cxx23) {
    test_skip("cxx23:mdspan::extents_type");
    test_skip("cxx23:mdspan::layout_type");
    test_skip("cxx23:mdspan::accessor_type");
    test_skip("cxx23:mdspan::mapping_type");
    test_skip("cxx23:mdspan::element_type");
    test_skip("cxx23:mdspan::value_type");
    test_skip("cxx23:mdspan::index_type");
    test_skip("cxx23:mdspan::size_type");
    test_skip("cxx23:mdspan::rank_type");
    test_skip("cxx23:mdspan::data_handle_type");
    test_skip("cxx23:mdspan::reference");
    test_skip("cxx23:mdspan");
    test_skip("cxx23:mdspan::rank");
    test_skip("cxx23:mdspan::rank_dynamic");
    test_skip("cxx23:mdspan::static_extent");
    test_skip("cxx23:mdspan::extent");
    test_skip("cxx23:mdspan::size");
    test_skip("cxx23:mdspan::extents");
    test_skip("cxx23:mdspan::mapping");
    test_skip("cxx23:mdspan::accessor");
    test_skip("cxx23:mdspan::is_always_unique");
    test_skip("cxx23:mdspan::is_always_exhaustive");
    test_skip("cxx23:mdspan::is_always_strided");
    test_skip("cxx23:mdspan::is_unique");
    test_skip("cxx23:mdspan::is_exhaustive");
    test_skip("cxx23:mdspan::is_strided");
    test_skip("cxx23:mdspan::mdspan (ctors)");
    test_skip("cxx23:mdspan::operator[]");
    test_skip("cxx23:mdspan::swap");
    test_skip("cxx23:mdspan deduction guides");
}
TEST_CASE(mdspan, submdspan_and_cxx26) {
    test_skip("cxx26:full_extent_t");
    test_skip("cxx26:full_extent");
    test_skip("cxx26:strided_slice");
    test_skip("cxx26:submdspan");
    test_skip("cxx26:submdspan_extents");
    test_skip("cxx26:submdspan_mapping_result");
    test_skip("cxx26:layout_left::submdspan_mapping");
    test_skip("cxx26:layout_right::submdspan_mapping");
    test_skip("cxx26:layout_stride::submdspan_mapping");
    test_skip("cxx26:subextents");
    test_skip("cxx26:canonical_slices");
    test_skip("cxx26:extent_slice");
    test_skip("cxx26:range_slice");
    test_skip("cxx26:dims");
    test_skip("cxx26:aligned_accessor");
    test_skip("cxx26:layout_left_padded");
    test_skip("cxx26:layout_right_padded");
    test_skip("cxx26:copy");
    test_skip("cxx26:fill");
}

#endif

TEST_CASE(mdspan, feature_test_macros) {
#if defined(__cpp_lib_mdspan)
    test_true( __cpp_lib_mdspan > 0L );
    test_pass("cxx23:__cpp_lib_mdspan");
#else
    test_skip("cxx23:__cpp_lib_mdspan");
#endif
#if defined(__cpp_lib_submdspan) && (__cpp_lib_submdspan) >= 202306L
    test_true( (__cpp_lib_submdspan) >= 202306L );
    test_pass("cxx26:__cpp_lib_submdspan=202306L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_submdspan=202306L");
#endif
#if defined(__cpp_lib_submdspan) && (__cpp_lib_submdspan) >= 202403L
    test_true( (__cpp_lib_submdspan) >= 202403L );
    test_pass("cxx26:__cpp_lib_submdspan=202403L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_submdspan=202403L");
#endif
#if defined(__cpp_lib_aligned_accessor)
    test_true( __cpp_lib_aligned_accessor > 0L );
    test_pass("cxx26:__cpp_lib_aligned_accessor");
#else
    test_skip("cxx26:__cpp_lib_aligned_accessor");
#endif
#if defined(__cpp_lib_freestanding_mdspan)
    test_true( __cpp_lib_freestanding_mdspan > 0L );
    test_pass("cxx26:__cpp_lib_freestanding_mdspan");
#else
    test_skip("cxx26:__cpp_lib_freestanding_mdspan");
#endif
}
