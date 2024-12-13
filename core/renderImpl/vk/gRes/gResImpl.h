#define XD_VK_RENDER_GENERATE_HOLDER_GETTER_IMPL(Type) \
    const GResHolder<Type::gResType>& Type::getImplInfo() const \
    { return getResHolder<gResType>(_holder); }