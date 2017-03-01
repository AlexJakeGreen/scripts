package ua.kh.bars.repositories;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;
import ua.kh.bars.entities.Ticket;

import java.util.List;

/**
 * Created by green on 3/18/16.
 */

@Repository
public interface TicketRepository extends JpaRepository<Ticket, Long> {

    List<Ticket> findAll();

}
