#pragma once

namespace UKControllerPlugin {
    namespace Controller {
        class ActiveCallsign;
        class ActiveCallsignCollection;
        class ControllerPosition;
        class ControllerPositionHierarchy;
    } // namespace Controller
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Handoff {
    class FlightplanSidHandoffMapper;
    class FlightplanAirfieldHandoffMapper;
    struct HandoffOrder;
    struct ResolvedHandoff;

    /**
     * Given a flightplan, resolves the handoff frequency
     * that should be used for the after departure TAG item.
     */
    class DepartureHandoffResolver
    {
        public:
        DepartureHandoffResolver(
            const FlightplanSidHandoffMapper& sidMapper,
            const FlightplanAirfieldHandoffMapper& airfieldMapper,
            const Controller::ActiveCallsignCollection& activeCallsigns);

        [[nodiscard]] auto Resolve(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
            -> std::shared_ptr<ResolvedHandoff>;

        private:
        [[nodiscard]] auto
        ResolveForHandoff(const Euroscope::EuroScopeCFlightPlanInterface& flightplan, const HandoffOrder& handoff) const
            -> std::shared_ptr<ResolvedHandoff>;
        [[nodiscard]] auto ResolveController(const HandoffOrder& handoff) const
            -> std::shared_ptr<const Controller::ControllerPosition>;
        [[nodiscard]] auto ResolveHandoff(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
            -> std::shared_ptr<ResolvedHandoff>;
        [[nodiscard]] static auto ResolveToUnicom(
            const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            std::shared_ptr<Controller::ControllerPositionHierarchy> handoffOrder) -> std::shared_ptr<ResolvedHandoff>;
        [[nodiscard]] static auto ResolvedToUnicom(const ResolvedHandoff& resolved) -> bool;

        // Maps flightplans to sids to handoffs
        const FlightplanSidHandoffMapper& sidMapper;

        // Maps flightplans to airfields to handoffs
        const FlightplanAirfieldHandoffMapper& airfieldMapper;

        // All the active controllers
        const Controller::ActiveCallsignCollection& activeCallsigns;

        // The unicom frequency
        inline static const double UNICOM_FREQUENCY = 122.800;
    };
} // namespace UKControllerPlugin::Handoff