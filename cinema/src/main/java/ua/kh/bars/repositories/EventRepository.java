package ua.kh.bars.repositories;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;
import ua.kh.bars.entities.Event;

import java.util.List;

/**
 * Created by green on 3/18/16.
 */

@Repository
public interface EventRepository extends JpaRepository<Event, Long> {

    List<Event> findAll();
}
