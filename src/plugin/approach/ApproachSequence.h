#pragma once
#include "ApproachSequencingMode.h"

namespace UKControllerPlugin::Approach {
    class ApproachSequencedAircraft;

    /**
     * Handles sequencing for aircraft on approach.
     */
    class ApproachSequence
    {
        public:
        void AddAircraftToSequence(const std::string& callsign, ApproachSequencingMode mode);
        void AddAircraftToSequence(
            const std::string& callsign, ApproachSequencingMode mode, const std::string& insertBefore);
        [[nodiscard]] auto First() const -> const std::shared_ptr<ApproachSequencedAircraft>;
        [[nodiscard]] auto Get(const std::string& callsign) const -> const std::shared_ptr<ApproachSequencedAircraft>;
        void RemoveAircraft(const std::string& callsign);

        private:
        [[nodiscard]] auto AircraftMatchingCallsign(const std::string& callsign) const
            -> std::list<std::shared_ptr<ApproachSequencedAircraft>>::const_iterator;
        std::list<std::shared_ptr<ApproachSequencedAircraft>> sequencedAircraft;
    };
} // namespace UKControllerPlugin::Approach
