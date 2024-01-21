#include <cubos/core/ecs/query/term.hpp>
#include <cubos/core/log.hpp>

using cubos::core::ecs::QueryTerm;

QueryTerm QueryTerm::makeEntity(int target)
{
    return {
        .type = DataTypeId::Invalid,
        .entity = {.target = target},
    };
}

QueryTerm QueryTerm::makeWithComponent(DataTypeId type, int target)
{
    return {
        .type = type,
        .component = {.target = target, .without = false, .optional = false},
    };
}

QueryTerm QueryTerm::makeWithoutComponent(DataTypeId type, int target)
{
    return {
        .type = type,
        .component = {.target = target, .without = true, .optional = false},
    };
}

QueryTerm QueryTerm::makeOptComponent(DataTypeId type, int target)
{
    return {
        .type = type,
        .component = {.target = target, .without = false, .optional = true},
    };
}

bool QueryTerm::isEntity() const
{
    return type == DataTypeId::Invalid;
}

bool QueryTerm::isComponent(const Types& types) const
{
    return type != DataTypeId::Invalid && types.isComponent(type);
}

bool QueryTerm::compare(const Types& types, const QueryTerm& other) const
{
    if (type != other.type)
    {
        return false;
    }

    if (this->isEntity())
    {
        return entity.target == other.entity.target;
    }
    else if (this->isComponent(types))
    {
        return component.target == other.component.target && component.without == other.component.without &&
               component.optional == other.component.optional;
    }
    else
    {
        CUBOS_UNREACHABLE();
    }
}

std::vector<QueryTerm> QueryTerm::resolve(const Types& types, const std::vector<QueryTerm>& baseTerms,
                                          std::vector<QueryTerm>& otherTerms)
{
    // This code assumes otherTerms comes from the query argument types, and thus, all of its targets
    // should be unspecified (-1).

    std::vector<QueryTerm> terms{};
    int defaultTarget = 0;

    // For each base term.
    auto otherTermIt = otherTerms.begin();
    for (auto baseTerm : baseTerms)
    {
        // Iterate over the remaining other terms until one with the same type is found.
        for (; otherTermIt != otherTerms.end(); ++otherTermIt)
        {
            auto& otherTerm = *otherTermIt;

            // The 'other' term has the same type as the 'base' term, we can treat them as one.
            if (baseTerm.type == otherTerm.type)
            {
                // Merge data from the other term into the base term.
                if (baseTerm.isEntity())
                {
                    CUBOS_ASSERT(otherTerm.entity.target == -1); // See comment at the beginning of the function.

                    if (baseTerm.entity.target == -1)
                    {
                        baseTerm.entity.target = defaultTarget;
                    }

                    otherTerm.entity.target = baseTerm.entity.target;
                    ++otherTermIt;
                    break;
                }
                else if (baseTerm.isComponent(types) && baseTerm.component.without == otherTerm.component.without)
                {
                    CUBOS_ASSERT(otherTerm.component.target == -1); // See comment at the beginning of the function.

                    if (baseTerm.entity.target == -1)
                    {
                        baseTerm.entity.target = defaultTarget;
                    }

                    baseTerm.component.optional = otherTerm.component.optional;
                    otherTerm.entity.target = baseTerm.entity.target;
                    ++otherTermIt;
                    break;
                }
            }

            // Change the other term's target to the default, as it was unspecified.
            if (otherTerm.isEntity())
            {
                CUBOS_ASSERT(otherTerm.entity.target == -1); // See comment at the beginning of the function.
                otherTerm.entity.target = defaultTarget;
            }
            else if (otherTerm.isComponent(types))
            {
                CUBOS_ASSERT(otherTerm.component.target == -1); // See comment at the beginning of the function.
                otherTerm.component.target = defaultTarget;
            }
            else
            {
                CUBOS_UNREACHABLE();
            }

            // Add it to the result.
            terms.emplace_back(otherTerm);
        }

        // Update either the defaultTarget or the term's target, depending on it being specified.
        if (baseTerm.isEntity())
        {
            if (baseTerm.entity.target == -1)
            {
                baseTerm.entity.target = defaultTarget;
            }
            else
            {
                defaultTarget = baseTerm.entity.target;
            }
        }
        else if (baseTerm.isComponent(types))
        {
            if (baseTerm.component.target == -1)
            {
                baseTerm.component.target = defaultTarget;
            }
            else
            {
                defaultTarget = baseTerm.component.target;
            }
        }
        else
        {
            CUBOS_UNREACHABLE();
        }

        // Add the base term to the result.
        terms.emplace_back(baseTerm);
    }

    // Add the remaining other terms.
    for (; otherTermIt != otherTerms.end(); ++otherTermIt)
    {
        auto& otherTerm = *otherTermIt;

        // Change the other term's target to the default, as it was unspecified.
        if (otherTerm.isEntity())
        {
            CUBOS_ASSERT(otherTerm.entity.target == -1); // See comment at the beginning of the function.
            otherTerm.entity.target = defaultTarget;
        }
        else if (otherTerm.isComponent(types))
        {
            CUBOS_ASSERT(otherTerm.component.target == -1); // See comment at the beginning of the function.
            otherTerm.component.target = defaultTarget;
        }
        else
        {
            CUBOS_UNREACHABLE();
        }

        // Add it to the result.
        terms.emplace_back(otherTerm);
    }

    return terms;
}