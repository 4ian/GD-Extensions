#ifndef LIGHTH
#define LIGHTH

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>
namespace sf
{
    class ConvexShape;
    class RenderTarget;
}

class Wall
{
public:
    Wall (sf::Vector2f p1,sf::Vector2f p2) : pt1(p1), pt2(p2) {}
    bool operator==(const Wall& other) { return ( pt1 == other.pt1 && pt2 == other.pt2 ); }

    sf::Vector2f pt1;
    sf::Vector2f pt2;
};

class GD_EXTENSION_API Light
{
    public :

    // Constructeur et destructeur
    Light();
    Light(sf::Vector2f position, float intensity, float radius, int quality, sf::Color color);
    ~Light();

    // Afficher la lumi�re
    void Draw(sf::RenderTarget *App);

    // Calculer la lumi�re
    virtual void Generate(std::vector <Wall*> &m_wall);

    // Ajouter un triangle � la lumi�re, en effet, les lumi�res sont compos�e de triangles
    void AddTriangle(sf::Vector2f pt1,sf::Vector2f pt2, int minimum_wall,std::vector <Wall*> &m_wall);

    // Changer diff�rents attributs de la lumi�re
    void SetIntensity(float intensity) { m_intensity=intensity; };
    void SetRadius(float radius) { m_radius=radius; };
    void SetQuality(int quality) { m_quality=quality; }
    void SetColor(sf::Color color) { m_color=color; }
    void SetPosition(sf::Vector2f position) {m_position=position;}

    // Retourner diff�rents attributs de la lumi�re
    float GetIntensity() const { return m_intensity; };
    float GetRadius() const { return m_radius; };
    int GetQuality() const { return m_quality; };
    sf::Color GetColor() const { return m_color; };
    sf::Vector2f GetPosition() const { return m_position; };

    // Une petite bool pour savoir si la lumi�re est allum�e ou �teinte
    bool m_actif;

    protected :
    //Position � l'�cran
    sf::Vector2f m_position;
    //Intensit�, g�re la transparence ( entre 0 et 255 )
    float m_intensity;
    //Rayon de la lumi�re
    float m_radius;
    //Couleur de la lumi�re
    sf::Color m_color;


    //Tableau dynamique de Shape, ce sont ces shapes de type triangle qui compose la lumi�re
    sf::VertexArray shapes; ///< The vertices composing the light
    //std::vector <sf::Vertex> m_shape;

    private :

    //Qualit� de la lumi�re, c'est � dire le nombre de triangles par d�faut qui la compose.
    int m_quality;
};

#endif


