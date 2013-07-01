/**

Game Develop - Timed Event Extension
Copyright (c) 2011-2013 Florian Rival (Florian.Rival@gmail.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.


*/
#if defined(GD_IDE_ONLY)

#include "GDCore/Events/Serialization.h"
#include "TimedEvent.h"
#include "GDCpp/RuntimeScene.h"
#include "GDCpp/tinyxml/tinyxml.h"
#include "GDCore/Events/EventsCodeGenerator.h"
#include "GDCore/Events/EventsCodeGenerationContext.h"
#include "GDCore/Events/ExpressionsCodeGeneration.h"
#include "GDCore/Events/Serialization.h"
#include "GDCore/IDE/EventsRenderingHelper.h"
#include "GDCore/IDE/EventsEditorItemsAreas.h"
#include "GDCore/IDE/EventsEditorSelection.h"
#include "TimedEventEditorDlg.h"

using namespace std;

std::vector< TimedEvent* > TimedEvent::codeGenerationCurrentParents;

TimedEvent::TimedEvent() :
BaseEvent()
{
}

TimedEvent::~TimedEvent()
{
}

vector < vector<gd::Instruction>* > TimedEvent::GetAllConditionsVectors()
{
    vector < vector<gd::Instruction>* > allConditions;
    allConditions.push_back(&conditions);

    return allConditions;
}

vector < vector<gd::Instruction>* > TimedEvent::GetAllActionsVectors()
{
    vector < vector<gd::Instruction>* > allActions;
    allActions.push_back(&actions);

    return allActions;
}

vector < gd::Expression* > TimedEvent::GetAllExpressions()
{
    vector < gd::Expression* > allExpressions;
    allExpressions.push_back(&timeout);

    return allExpressions;
}

void TimedEvent::SaveToXml(TiXmlElement * eventElem) const
{
    TiXmlElement * objectElem = new TiXmlElement( "Name" );
    eventElem->LinkEndChild( objectElem );
    objectElem->SetAttribute("value", name.c_str());

    TiXmlElement * timeoutElem = new TiXmlElement( "Timeout" );
    eventElem->LinkEndChild( timeoutElem );
    timeoutElem->SetAttribute("value", timeout.GetPlainString().c_str());

    //Les conditions
    TiXmlElement * conditionsElem = new TiXmlElement( "Conditions" );
    eventElem->LinkEndChild( conditionsElem );
    gd::EventsListSerialization::SaveConditions(conditions, conditionsElem);

    //Les actions
    TiXmlElement * actionsElem = new TiXmlElement( "Actions" );
    eventElem->LinkEndChild( actionsElem );
    gd::EventsListSerialization::SaveActions(actions, actionsElem);

    //Sous �v�nements
    if ( !GetSubEvents().empty() )
    {
        TiXmlElement * subeventsElem;
        subeventsElem = new TiXmlElement( "Events" );
        eventElem->LinkEndChild( subeventsElem );

        gd::EventsListSerialization::SaveEventsToXml(events, subeventsElem);
    }
}

void TimedEvent::LoadFromXml(gd::Project & project, const TiXmlElement * eventElem)
{
    if ( eventElem->FirstChildElement( "Name" ) != NULL )
        name = eventElem->FirstChildElement("Name")->Attribute("value");

    if ( eventElem->FirstChildElement( "Timeout" ) != NULL )
        timeout = gd::Expression(eventElem->FirstChildElement("Timeout")->Attribute("value"));

    //Conditions
    if ( eventElem->FirstChildElement( "Conditions" ) != NULL )
        gd::EventsListSerialization::OpenConditions(project, conditions, eventElem->FirstChildElement( "Conditions" ));
    else
        cout << "Aucune informations sur les conditions d'un �v�nement";

    //Actions
    if ( eventElem->FirstChildElement( "Actions" ) != NULL )
        gd::EventsListSerialization::OpenActions(project, actions, eventElem->FirstChildElement( "Actions" ));
    else
        cout << "Aucune informations sur les actions d'un �v�nement";

    //Subevents
    if ( eventElem->FirstChildElement( "Events" ) != NULL )
        gd::EventsListSerialization::LoadEventsFromXml(project, events, eventElem->FirstChildElement( "Events" ));
}

/**
 * Render the event in the bitmap
 */
void TimedEvent::Render(wxDC & dc, int x, int y, unsigned int width, EventsEditorItemsAreas & areas, EventsEditorSelection & selection, const gd::Platform & platform)
{
    gd::EventsRenderingHelper * renderingHelper = gd::EventsRenderingHelper::GetInstance();
    int border = renderingHelper->instructionsListBorder;
    const int functionTextHeight = 20;

    //Draw header rectangle
    wxRect headerRect(x, y, width, functionTextHeight);
    renderingHelper->DrawNiceRectangle(dc, headerRect);

    //Name
    dc.SetFont( renderingHelper->GetNiceFont().Bold()  );
    dc.SetTextForeground(wxColour(0,0,0));
    std::string nameTxt;
    if ( !name.empty() ) nameTxt = _T(" (Nom : "+name + ")");
    dc.DrawText( _("Delayed execution after ")+timeout.GetPlainString()+_(" seconds.")+" "+nameTxt, x + 4, y + 3 );

    //Draw conditions rectangle
    wxRect rect(x, y+functionTextHeight, renderingHelper->GetConditionsColumnWidth()+border, GetRenderedHeight(width, platform)-functionTextHeight);
    renderingHelper->DrawNiceRectangle(dc, rect);

    //Draw actions and conditions
    renderingHelper->DrawConditionsList(conditions, dc,
                                        x+border,
                                        y+functionTextHeight+border,
                                        renderingHelper->GetConditionsColumnWidth()-border, this, areas, selection, platform);
    renderingHelper->DrawActionsList(actions, dc,
                                     x+renderingHelper->GetConditionsColumnWidth()+border,
                                     y+functionTextHeight+border,
                                     width-renderingHelper->GetConditionsColumnWidth()-border*2, this, areas, selection, platform);
}

unsigned int TimedEvent::GetRenderedHeight(unsigned int width, const gd::Platform & platform) const
{
    if ( eventHeightNeedUpdate )
    {
        gd::EventsRenderingHelper * renderingHelper = gd::EventsRenderingHelper::GetInstance();
        int border = renderingHelper->instructionsListBorder;
        const int functionTextHeight = 20;

        //Get maximum height needed
        int conditionsHeight = renderingHelper->GetRenderedConditionsListHeight(conditions, renderingHelper->GetConditionsColumnWidth()-border*2, platform);
        int actionsHeight = renderingHelper->GetRenderedActionsListHeight(actions, width-renderingHelper->GetConditionsColumnWidth()-border*2, platform);

        renderedHeight = (( conditionsHeight > actionsHeight ? conditionsHeight : actionsHeight ) + functionTextHeight)+border*2;
        eventHeightNeedUpdate = false;
    }

    return renderedHeight;
}

gd::BaseEvent::EditEventReturnType TimedEvent::EditEvent(wxWindow* parent, gd::Project & game, gd::Layout & scene, gd::MainFrameWrapper & mainFrameWrapper)
{
    TimedEventEditorDlg dialog(parent, *this, game, scene);
    if ( dialog.ShowModal() == 0 ) return Cancelled;

    return ChangesMade;
}

/**
 * Initialize from another TimedEvent.
 * Used by copy ctor and assignement operator
 */
void TimedEvent::Init(const TimedEvent & event)
{
    events = CloneVectorOfEvents(event.events);

    name = event.name;
    timeout = event.timeout;
    conditions = event.conditions;
    actions = event.actions;
}

/**
 * Custom copy operator
 */
TimedEvent::TimedEvent(const TimedEvent & event) :
BaseEvent(event)
{
    Init(event);
}

/**
 * Custom assignement operator
 */
TimedEvent& TimedEvent::operator=(const TimedEvent & event)
{
    if ( this != &event )
    {
        BaseEvent::operator=(event);
        Init(event);
    }

    return *this;
}

#endif
