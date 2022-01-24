#include "material.h"

Phong::Phong(const PropertyList &propList) : Diffuse(propList.getColor("diffuse", Color3f(.2)))
{
    m_specularColor = propList.getColor("specular", Color3f(.9f));
    m_reflectivity = propList.getColor("reflectivity", Color3f(0.f));
    m_exponent = propList.getFloat("exponent", .2f);

    std::string texturePath = propList.getString("texture", "");
    if (texturePath.size() > 0)
    {
        filesystem::path filepath = getFileResolver()->resolve(texturePath);
        loadTextureFromFile(filepath.str());
        setTextureScale(propList.getFloat("scale", 1.f));
        setTextureMode(TextureMode(propList.getInteger("mode", 0)));
    }
}

Color3f Phong::brdf(const Vector3f &viewDir, const Vector3f &lightDir, const Normal3f &normal, const Vector2f &uv) const
{
    const Color3f diffuse = m_diffuseColor;

    const Vector3f r = 2 * normal.dot(lightDir) * normal - lightDir;
    const Color3f specular = m_specularColor * pow(std::max(r.dot(viewDir), 0.f), m_exponent);

    return diffuse + specular;
}

std::string Phong::toString() const
{
    return tfm::format(
        "Phong[\n"
        "  diffuse color = %s\n"
        "  specular color = %s\n"
        "  exponent = %f\n"
        "]",
        m_diffuseColor.toString(),
        m_specularColor.toString(),
        m_exponent);
}

REGISTER_CLASS(Phong, "phong");
