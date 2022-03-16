#include "AircraftSelectionProvider.h"
#include "ApproachBootstrapProvider.h"
#include "ApproachModuleFactory.h"
#include "ApproachSequencerDisplay.h"
#include "ApproachSequencerDisplayAsrLoader.h"
#include "ApproachSequencerDisplayOptions.h"
#include "ApproachSpacingRingRenderer.h"
#include "SequencerAirfieldSelector.h"
#include "TargetSelectorList.h"
#include "ToggleApproachSequencerDisplay.h"
#include "aircraft/CallsignSelectionListFactory.h"
#include "bootstrap/ModuleFactories.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "list/PopupListFactory.h"
#include "radarscreen/MenuToggleableDisplayFactory.h"
#include "radarscreen/RadarRenderableCollection.h"

namespace UKControllerPlugin::Approach {

    void ApproachBootstrapProvider::BootstrapPlugin(Bootstrap::PersistenceContainer& container)
    {
    }

    void ApproachBootstrapProvider::BootstrapRadarScreen(
        const Bootstrap::PersistenceContainer& container,
        RadarScreen::RadarRenderableCollection& radarRenderables,
        RadarScreen::ConfigurableDisplayCollection& configurables,
        Euroscope::AsrEventHandlerCollection& asrHandlers,
        const RadarScreen::MenuToggleableDisplayFactory& toggleableDisplayFactory)
    {
        // Sequencer display
        auto sequencerRendererId = radarRenderables.ReserveRendererIdentifier();
        auto sequencerScreenObjectId = radarRenderables.ReserveScreenObjectIdentifier(sequencerRendererId);

        auto displayOptions = std::make_shared<ApproachSequencerDisplayOptions>();
        radarRenderables.RegisterRenderer(
            sequencerRendererId,
            std::make_shared<ApproachSequencerDisplay>(
                container.moduleFactories->Approach().Sequencer(),
                container.moduleFactories->Approach().SpacingCalculator(container),
                displayOptions,
                container.popupListFactory->Create(
                    std::make_shared<SequencerAirfieldSelector>(displayOptions, *container.airfields),
                    "Toggle sequencer airfield selector"),
                container.callsignSelectionListFactory->Create(
                    std::make_shared<AircraftSelectionProvider>(
                        container.moduleFactories->Approach().Sequencer(), displayOptions, *container.plugin),
                    "Toggle sequencer callsign selector"),
                container.popupListFactory->Create(
                    std::make_shared<TargetSelectorList>(
                        container.moduleFactories->Approach().Sequencer(), displayOptions, *container.plugin),
                    "Toggle sequencer target selector"),
                *container.plugin,
                sequencerScreenObjectId),
            radarRenderables.beforeTags);

        asrHandlers.RegisterHandler(std::make_shared<ApproachSequencerDisplayAsrLoader>(displayOptions));
        toggleableDisplayFactory.RegisterDisplay(
            std::make_shared<ToggleApproachSequencerDisplay>(displayOptions), "Approach sequencer display toggle");

        // Ring renderer
        radarRenderables.RegisterRenderer(
            radarRenderables.ReserveRendererIdentifier(),
            std::make_shared<ApproachSpacingRingRenderer>(
                container.moduleFactories->Approach().Sequencer(),
                container.moduleFactories->Approach().SpacingCalculator(container),
                displayOptions,
                *container.plugin),
            radarRenderables.beforeTags);
    }
} // namespace UKControllerPlugin::Approach
